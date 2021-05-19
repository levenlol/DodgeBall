import numpy as np
import tensorflow as tf

import tensorflow_probability as tfp

from tensorflow.keras.layers import Dense, Input
from tensorflow.keras.models import Model


def get_critic(input_space):
    i = Input(shape=(input_space,))
    x = Dense(128, activation='relu')(i)
    x = Dense(256, activation='relu')(x)
    x = Dense(512, activation='relu')(x)

    val = Dense(1)(x)
    model = Model(i, val)

    return model

def get_actor(input_space, action_space):
    i = Input(shape=(input_space,))
    x = Dense(128, activation='relu')(i)
    x = Dense(256, activation='relu')(x)
    #x = Dense(512, activation='relu')(x)

    up_val = Dense(action_space, activation='softmax')(x)
    right_val = Dense(action_space, activation='softmax')(x)

    model = Model(i, [up_val, right_val])

    return model

class Agent:
    def __init__(self, input_space, action_space, learning_rate = 0.0001):
        self.actor = get_actor(input_space, action_space)
        self.critic = get_critic(input_space)

        self.actor_opt = tf.keras.optimizers.Adam(learning_rate)
        self.critic_opt = tf.keras.optimizers.Adam(learning_rate)

    def get_action(self, state):
        up, right = self.actor(np.array([state]))
        prob_up = up.numpy()
        prob_right = right.numpy()

        dist_up = tfp.distributions.Categorical(probs=prob_up, dtype=tf.float32)
        dist_right = tfp.distributions.Categorical(probs=prob_right, dtype=tf.float32)
        action_up, action_right = dist_up.sample(), dist_right.sample()
        return int(action_up.numpy()[0]), int(action_right.numpy()[0])

    def actor_loss(self, probs, actions, rewards, critic_value):
        probability = []
        log_probability = []

        for pb, a in zip(probs, actions):
            dist = tfp.distributions.Categorical(probs=pb, dtype=tf.float32)
            prob = dist.prob(a)
            log_prob = dist.log_prob(a) 

            probability.append(prob)
            log_probability.append(log_prob)

        p_loss = []
        e_loss = []

        c = tf.reshape(critic_value, (len(critic_value),))
        advantage = tf.subtract(rewards, c)

        for pb, adv, lpb in zip(probability, advantage, log_probability):
            policy_loss = tf.math.multiply(adv, lpb)
            entropy_loss = tf.math.negative(tf.math.multiply(pb, lpb))

            p_loss.append(policy_loss)
            e_loss.append(entropy_loss)

        p_loss = tf.reduce_mean(tf.stack(p_loss))
        e_loss = tf.reduce_mean(tf.stack(e_loss))

        loss = -(p_loss + 0.0001 * e_loss)
        return loss

    def learn(self, states, actions, rewards):

        with tf.GradientTape() as tape1, tf.GradientTape() as tape2:
            p = self.actor(states, training=True)
            c = self.critic(states, training=True)

            a_loss = self.actor_loss(p, actions, rewards, c)
            c_loss = 0.5 * tf.keras.losses.mean_squared_error(c, rewards)

        grads1 = tape1.gradient(a_loss, self.actor.trainable_variables)
        grads2 = tape2.gradient(c_loss, self.critic.trainable_variables)

        self.actor_opt.apply_gradients(zip(grads1, self.actor.trainable_variables))
        self.critic_opt.apply_gradients(zip(grads2, self.critic.trainable_variables))
        return a_loss, c_loss




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




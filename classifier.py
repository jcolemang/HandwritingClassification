import pandas as pd
import numpy as np

def sigmoid(x, deriv=False):
    if(deriv==True):
        return x*(1-x)
    return 1/(1+np.exp(-x))
                    

class Classifier(object):
    def __init__(self):
        self.syn0 = np.array(pd.read_csv("3-Layer Sigmoid-Softmax NN syn0 2.5X.csv").drop("Unnamed: 0", axis = 1)).astype(float)
        self.syn1 = np.array(pd.read_csv("3-Layer Sigmoid-Softmax NN syn1 2.5X.csv").drop("Unnamed: 0", axis = 1)).astype(float)

    def predict( self, vector ):
        #print 'Syn:', self.syn0
        #print 'Vector:', vector
        hidden = sigmoid(np.dot(vector,self.syn0))
        exp_scores = np.exp(np.dot(hidden, self.syn1))
        probs = exp_scores / np.sum(exp_scores, axis=0, keepdims=True)
        output = probs
        return output.argmax(axis=0)

    def train(self):
        pass

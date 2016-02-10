"""

All credit to John Hien. This was pretty much copied from 
his ipython notebook found in the iPythonNotebookFiles/ 
directory. The current state of this program corresponds 
to the notebook under in 1.58\ %\ Error\ NN\ Structure/ 
subdirectory.

"""


import pandas as pd
import numpy as np

layer_data_path = './ClassifierData/'

# some functions for some stuff I bet is important -James

def sigmoid(x, deriv=False):
    if(deriv==True):
        return x*(1-x)
    return 1/(1+np.exp(-x))
                    

def rectifier(x, deriv=False):
    flag = x > 0
    if deriv == True:
        return 1 if flag else 0
    return x if flag else 0


def softmax(x, deriv=False):
    if deriv == True:
        return 1
    exp_scores = np.exp(x)
    return exp_scores / np.sum(exp_scores, axis=1, keepdims=True)


def tanh():
    if deriv == True:
        return 1 - np.power(x, 2)
    return np.tanh(x)


# A wrapped up version of the classifier for whichever one we 
# may be using at the moment.
class Classifier(object):
    def __init__(self):
        self.input_layer_size = 784 
        self.num_hidden_layers = 3
        self.hidden_layer_size = 1024
        self.output_layer_size = 10
        self.hidden_layer_function = sigmoid
        self.output_layer_function = softmax
        self.synapse = []
        self.biases = []
        for index in range(self.num_hidden_layers + 1):
            syn_filename = "%d-Layer %s-%s %d-%d-%d nodes syn%d.csv" % (
                    self.num_hidden_layers, self.hidden_layer_function.func_name, 
                    self.output_layer_function.func_name, self.input_layer_size, 
                    self.hidden_layer_size, self.output_layer_size, index)
            syn_filename = layer_data_path + syn_filename
            bias_filename = "%d-Layer %s-%s %d-%d-%d nodes bias%d.csv" % (
                    self.num_hidden_layers, self.hidden_layer_function.func_name, 
                    self.output_layer_function.func_name, self.input_layer_size, 
                    self.hidden_layer_size, self.output_layer_size, index)
            bias_filename = layer_data_path + bias_filename
            self.synapse.append(np.array(pd.read_csv(syn_filename).drop("Unnamed: 0", axis = 1)))
            self.biases.append(np.array(pd.read_csv(bias_filename).drop("Unnamed: 0", axis = 1)))

    def predict(self, to_predict):
        current_layer = to_predict

        for layer in range(self.num_hidden_layers):

            current_layer = self.hidden_layer_function( \
                    np.dot(current_layer, self.synapse[layer]) + self.biases[layer].T)

        output = self.output_layer_function( \
                np.dot( current_layer, self.synapse[self.num_hidden_layers] ) \
                + self.biases[self.num_hidden_layers].T)

        answer = np.zeros( len(output), dtype=np.int8 )
        for i in range(len(output)):
            answer[i] = output[i].argmax()
        return answer




















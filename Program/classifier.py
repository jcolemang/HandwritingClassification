"""

This file contains the neural network classifier used by the program.
The default structure is saved in .csv files and all the functions reflect
these defauls. This file also contains the functionality to design and train
a new neural network structure from scratch, or even to alter a pretrained
network.

Contributors: James Gibson, Johnathon Hein
Last Edited: 2/10/2016
"""


import pandas as pd
import numpy as np

layer_data_path = './ClassifierData/'

"""

Different functions which are avaliable to be used in the neural network classifier.
sigmoid/sofmax/tanh - work well
ReLU - has not been shown to work yet

"""
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

"""
A wrapped up version of the classifier for whichever one we 
may be using at the moment.
"""
class Classifier(object):
	def __init__(self, pre_trained=True, input_layer_size=784,num_hidden_layers=3,hidden_layer_size=1024, \
				  output_layer_size=10,hidden_layer_function=sigmoid,output_layer_function=softmax):
		self.input_layer_size = input_layer_size 
		self.num_hidden_layers = num_hidden_layers
		self.hidden_layer_size = hidden_layer_size
		self.output_layer_size = output_layer_size
		self.hidden_layer_function = hidden_layer_function
		self.output_layer_function = output_layer_function
		self.synapse = []
		self.biases = []
		self.synapse_deltas = []
		self.bias_deltas = []
		self.layers = []
		self.setTrainingParams()
		if(pre_trained):
			self.synapse = []
			self.biases = []
			self.synapse_deltas = []
			self.bias_deltas = []
			self.dropout=False
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
		else:
			self.reset_params()
			self.dropout=True

	"""
	Predicts the digit labels of the inputs. Input must be in the shape of (*,self.input_layer_size)
	"""
	def predict(self, input):
		current_layer = input

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

	"""
	Returns the neural network output based on an input
	"""
	def get_output(self, input):
		self.layers = []
		self.layers.append(input)
		if(self.dropout):
			self.layers[0] *= np.random.binomial([np.ones(self.layers[0].shape)], self.input_dropout)[0]
			
		for layer in range(self.num_hidden_layers):
			self.layers.append(self.hidden_layer_function(np.dot(self.layers[-1], self.synapse[layer]) + \
				self.biases[layer].T))
			if(self.dropout):
				self.layers[layer + 1] *= np.random.binomial([np.ones(self.layers[layer+1].shape)], \
															 self.hidden_dropout)[0]     

		self.layers.append(self.output_layer_function(np.dot(self.layers[-1], self.synapse[self.num_hidden_layers] ) \
			+ self.biases[self.num_hidden_layers].T))

		return self.layers[-1]

	"""
	Update the synapses and bias of this network
	"""
	def update(self):
		# Update synapses
		for index in range(len(self.synapse)):
			self.synapse[index] -= self.synapse_deltas[index]
			
		# Update biases
		for index in range(len(self.biases)):
			self.biases[index] -= self.bias_deltas[index]
		
		# Find magnitude of neuron vectors and preform max-normalization
		if(self.max_norm):
			for i in range(len(self.synapse)):
				syn_mag = np.linalg.norm(self.synapse[i], axis = 0)
				if(any(syn_mag > self.c)):
					self.synapse[i] = self.synapse[i] / (syn_mag / self.c)

	"""
	Reset the synapse and bias values of this network
	"""
	def reset_params(self):
		self.synapse = []
		self.biases = []
		self.synapse_deltas = []
		self.bias_deltas = []
		
		# Input-Hidden synapse
		self.synapse.append(2 * np.random.random((self.input_layer_size, self.hidden_layer_size)) - 1)
		self.biases.append(np.zeros(self.hidden_layer_size))
		self.synapse_deltas.append(np.zeros((self.input_layer_size, self.hidden_layer_size)))
		self.bias_deltas.append(np.zeros(self.hidden_layer_size))

		# Hidden-Hidden synapse
		for layer in range(1, self.num_hidden_layers):
			self.synapse.append(2 * np.random.random((self.hidden_layer_size, self.hidden_layer_size)) - 1)
			self.biases.append(np.zeros(self.hidden_layer_size))
			self.synapse_deltas.append(np.zeros((self.hidden_layer_size, self.hidden_layer_size)))
			self.bias_deltas.append(np.zeros(self.hidden_layer_size))

		# Hidden-Output synapse
		self.synapse.append(2 * np.random.random((self.hidden_layer_size, self.output_layer_size)) - 1)
		self.biases.append(np.zeros(self.output_layer_size))
		self.synapse_deltas.append(np.zeros((self.hidden_layer_size, self.output_layer_size)))
		self.bias_deltas.append(np.zeros(self.output_layer_size))

	"""
	Sets the training parameters for the NN
	"""
	def setTrainingParams(self, learning_rate=0.0028,momentum=0.95,max_norm=True,c=10,hidden_dropout=0.5, \
						  input_dropout=0.80,synapse_size=1,bias_size=1):
		# Hyperparameters for learning
		self.learning_rate = learning_rate
		self.momentum = momentum
		self.synapse_size = synapse_size
		self.bias_size = bias_size
		self.max_norm = max_norm
		self.c = c
		self.hidden_dropout = hidden_dropout
		self.input_dropout = input_dropout
		
	"""
	Performs stoichastic minibatch training with the input data
	"""	
	def minibatch_train(self, train_data, train_target, training_steps=1, mini_batch_size=100):
		# Multiple training runs Stoichiastic training of the neural network
		for iteration in xrange(training_steps):
			# Create a set of minibatches for training
			all_inds = np.array(train_data.index)
			np.random.shuffle(all_inds)
			batch_inds = [all_inds[i:i+mini_batch_size] for i in xrange(0, len(all_inds), mini_batch_size)]
			# train on each batch
			for inds in batch_inds:
				X = np.array(train_data.ix[inds])
				y = np.array(train_target.ix[inds])
				self.train(X,y)

	"""
	Regular training on train_data over the desired number of steps
	"""
	def train(self, train_data, train_target, training_steps=1):
		# Multiple training runs Stoichiastic training of the neural network
		for iteration in xrange(training_steps):
			X = np.array(train_data)
			y = np.array(train_target)

			self.get_output(X)
			
			layer_deltas = [None] * (self.num_hidden_layers+1)
			
			# Backpropogation: Layer contribution to errors
			layer_deltas[-1] = (self.layers[-1] - y) * self.output_layer_function(self.layers[-1], True)
			for layer in range(2, self.num_hidden_layers + 2):
				layer_deltas[-layer] = layer_deltas[-(layer - 1)].dot(self.synapse[-(layer - 1)].T) \
										* self.hidden_layer_function(self.layers[-layer], True)
				
			# Get changes for synapses and biases
			for index in range(len(self.synapse)):
				self.synapse_deltas[index] = self.layers[index].T.dot(layer_deltas[index]) * self.learning_rate \
					+ self.momentum*self.synapse_deltas[index]
			for index in range(len(self.biases)):
				self.bias_deltas[index] = np.ones(len(X)).dot(layer_deltas[index]) * self.learning_rate \
										   + self.momentum*self.bias_deltas[index]
			
			# Update parameters
			self.update()
			
	"""
	Turn on/off the drouput functionality in this network.
	Default is on when the network is not pretrained
	"""
	def setDropout(do_dropout):
		if(self.do_dropout and not do_dropout):
			# Scale down the synapses to accomadate using the network without dropout
			if do_dropout:
				self.synapse[0] *= self.input_dropout
				for i in range(1, len(self.synapse)):
					self.synapse[i] *= self.hidden_dropout

		self.dropout = do_dropout
		
	"""
	Save the syanpses and bias of this network to .csv files
	"""
	def saveClassifier(self):
		# Save the synapses for later use
		for index in range(len(self.synapse)):
			snaypse_i = pd.DataFrame(self.synapse[index])
			bias_i = pd.DataFrame(self.biases[index])
			snaypse_i.to_csv("%d-Layer %s-%s %d-%d-%d nodes syn%d.csv" % (self.num_hidden_layers, \
				self.hidden_layer_function.func_name, self.output_layer_function.func_name, self.input_layer_size, \
				self.hidden_layer_size, self.output_layer_size, index))
			bias_i.to_csv("%d-Layer %s-%s %d-%d-%d nodes bias%d.csv" % (self.num_hidden_layers, \
				self.hidden_layer_function.func_name, self.output_layer_function.func_name, self.input_layer_size, \
				self.hidden_layer_size, self.output_layer_size, index))


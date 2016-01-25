import numpy as np

# sigmoid function
def sigmoid(x,deriv=False):
    if(deriv==True):
        return x*(1-x)
    return 1/(1+np.exp(-x))
    
# input dataset
train_data = np.array([ [0,0,1],
                        [0,1,1],
                        [1,0,1],
                        [1,1,1] ])

ERROR_THRESHOLD = 0.005
    
    # output dataset            
train_target = np.array([[0],
                         [1],
                         [1],
                         [0]])

# seed random numbers to make calculation
# deterministic (just a good practice)
np.random.seed(1)

# initialize weights randomly with mean 0
syanpse_0 = 2*np.random.random((3,4)) - 1
syanpse_1 = 2*np.random.random((4,1)) - 1

# iterate many times with backpropogation
for j in xrange(60000):

    # forward propagation
    input_layer = train_data
    hidden_layer = sigmoid(np.dot(input_layer,syanpse_0))
    output_layer = sigmoid(np.dot(hidden_layer,syanpse_1))

    # how much did we miss?
    output_layer_error = train_target - output_layer

    if (j% 10000) == 0:
        print "Error:" + str(np.mean(np.abs(output_layer_error)))
    
    # end iteration once error approaches threshold
    #max_error = np.max(np.abs(output_layer_error))
    #if max_error <= ERROR_THRESHOLD: break
    
    # in what direction is the target value?
    # were we really sure? if so, don't change too much.
    output_layer_delta = output_layer_error*sigmoid(output_layer,deriv=True)
    
    hidden_layer_error = output_layer_delta.dot(syanpse_1.T)
    
    # multiply how much we missed by the 
    # slope of the sigmoid at the values in l1
    hidden_layer_delta = hidden_layer_error * sigmoid(hidden_layer,True)

    # update weights
    syanpse_1 += hidden_layer.T.dot(output_layer_delta)
    syanpse_0 += input_layer.T.dot(hidden_layer_delta)

def pred(input):
    hidden = sigmoid(np.dot(input,syanpse_0))
    output = sigmoid(np.dot(hidden,syanpse_1))
    return output

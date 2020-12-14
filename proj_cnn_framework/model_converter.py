import torch
from torch import nn, optim
import torch.nn.functional as F
from torchvision import transforms
from torchvision import datasets
import json
import numpy as np

class NumpyEncoder(json.JSONEncoder):
    def default(self, obj):
        if isinstance(obj, np.ndarray):
            return obj.tolist()
        return json.JSONEncoder.default(self, obj)

class Net(nn.Module):
   def __init__(self):
       super().__init__()
       self.conv1 = nn.Conv2d(1, 6, kernel_size=(5,5)) # input channel = 1; output channel = 6; kernel size = (5,5); 
       self.conv2 = nn.Conv2d(6, 16, kernel_size=(3,3)) # input channel = 6; output channel = 16; kernel size = (3,3); 
       self.fc1 = nn.Linear(16*5*5, 120)
       self.fc2 = nn.Linear(120, 84)
       self.fc3 = nn.Linear(84, 10)
   def forward(self, x):
       x = F.max_pool2d(F.relu(self.conv1(x)), (2, 2)) # (1, 28, 28) -> (6, 24, 24) -> (6, 12, 12)
       x = F.max_pool2d(F.relu(self.conv2(x)), (2, 2)) # (6, 12, 12) -> (16, 10, 10) -> (16, 5, 5)
       x = x.view(-1, self.num_flat_features(x)) # (16, 5, 5) -> (16*5*5)
       x = F.relu(self.fc1(x)) # (16*5*5 -> 120)
       x = F.relu(self.fc2(x)) # (120 -> 84)
       x = self.fc3(x) # (84 -> 10)
       return x
   def num_flat_features(self, x):
       size = x.size()[1:]
       num_features = 1
       for s in size:
           num_features *= s
       return num_features

model = Net()

print("Load the trained model state......\n")
model.load_state_dict(torch.load('./Net.pth'))

para_dict = {}

print("====== Layer : Parameter Shape ======")
for name, param in model.state_dict().items():
    print(name, ":", param.numpy().shape)
    para_dict[name] = param.numpy()

print("\nSave the parameters in JSON file......")
with open('param.json', 'w') as f:
    json.dump(para_dict, f, cls=NumpyEncoder)
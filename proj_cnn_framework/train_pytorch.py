import torch
from torch import nn, optim
import torch.nn.functional as F
from torch.autograd import Variable
from torch.utils.data import DataLoader
from torchvision import transforms
from torchvision import datasets

# 定义超参数
batch_size = 128        # 批的大小
learning_rate = 1e-2    # 学习率
num_epoches = 20        # 遍历训练集的次数

# 下载训练集 MNIST 手写数字训练集
train_dataset = datasets.MNIST(
    root='./data', train=True, transform=transforms.ToTensor(), download=True)

test_dataset = datasets.MNIST(
    root='./data', train=False, transform=transforms.ToTensor())

train_loader = DataLoader(train_dataset, batch_size=batch_size, shuffle=True)
test_loader = DataLoader(test_dataset, batch_size=batch_size, shuffle=False)


# 定义 Convolution Network 模型
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

# 定义loss和optimizer
criterion = nn.CrossEntropyLoss()
optimizer = optim.SGD(model.parameters(), lr=learning_rate)

# 开始训练
for epoch in range(num_epoches):
    print('epoch {}'.format(epoch + 1))      # .format为输出格式，formet括号里的即为左边花括号的输出
    print('*' * 10)
    running_loss = 0.0
    running_acc = 0.0
    for i, data in enumerate(train_loader, 1):
        img, label = data
        # 向前传播
        out = model(img)
        loss = criterion(out, label)
        running_loss += loss.item() * label.size(0)
        _, pred = torch.max(out, 1)
        num_correct = (pred == label).sum()
        accuracy = (pred == label).float().mean()
        running_acc += num_correct.item()
        # 向后传播
        optimizer.zero_grad()
        loss.backward()
        optimizer.step()

    print('Finish {} epoch, Loss: {:.6f}, Acc: {:.6f}'.format(
        epoch + 1, running_loss / (len(train_dataset)), running_acc / (len(train_dataset))))

    eval_loss = 0
    eval_acc = 0

    with torch.no_grad():
        for data in test_loader:
            img, label = data
            out = model(img)
            loss = criterion(out, label)
            eval_loss += loss.item() * label.size(0)
            _, pred = torch.max(out, 1)
            num_correct = (pred == label).sum()
            eval_acc += num_correct.item()
        print('Test Loss: {:.6f}, Acc: {:.6f}'.format(eval_loss / (len(
            test_dataset)), eval_acc / (len(test_dataset))))
        print()

# 保存模型
torch.save(model.state_dict(), './Net.pth')
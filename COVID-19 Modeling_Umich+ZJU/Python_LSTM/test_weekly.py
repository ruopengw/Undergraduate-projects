
import torch
import matplotlib.pyplot as plt
from function import loss_cal, adjust_learning_rate, Net
from parameter_weekly import feature_num
import numpy as np

path = 'checkpoint_weekly_sequential.tar'


def test_weekly(path, test_x, test_y, train_mean, train_std):

    model = Net(feature_num)
    # using the test dataset to test model
    checkpoint = torch.load(path)
    model.load_state_dict(checkpoint['net'])
    model.eval()
    predict = model(test_x)
    real = test_y.clone()

    loss_fun = torch.nn.MSELoss()
    # two losses, one is normalized scale, another is original scale
    loss = loss_fun(predict, real)
    loss_original_scale, pre, rea = loss_cal(predict, real, train_mean, train_std)
    pre = np.array(pre.data)
    rea = np.array(rea.data)
    print("Test loss: " + str(loss))
    print("Test loss in original scale: " + str(loss_original_scale))

    # true output and predicted output
    plt.figure(2)
    plt.plot(list(rea), label="real")
    plt.plot(list(pre), label="pred")
    plt.legend(loc='best')
    plt.xlabel('test points')
    plt.ylabel('new cases')
    plt.show()

    # true output and predicted output
    plt.figure(3)
    min_val = min(rea)
    max_val = max(rea)
    plt.scatter(rea,pre)
    plt.plot([min_val,max_val],[min_val,max_val],color = 'red')
    plt.ylabel('predicted new cases')
    plt.xlabel('real new cases')
    plt.show()
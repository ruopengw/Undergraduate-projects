"""
In this project, I created a LSTM model to predict covid-19 weekly new_cases in LosAngeles.

Some super parameters of this coding:

- week_input    : how many weeks' feature we use to predict
- timelagging  : the length of latent window of Social Distancing data & Mobility data
- feature_num  : the number of feature that one input contains
- average_num  : how many weeks does the average cases get from

The timelagging effect is only in exist in Social Distancing data and Mobility data.
Currently, the time lagging of first week's Social Distancing data is one weeks.

Input and output of LSTM:

- input:    the size of input [bathch_size, 1, feature_num]
            currently, the features one point contains are:
                        confirmed_cases                             week1
                        new_confirmed_cases                         week1
                        device_count                                week1
                        istance_traveled_from_home                  week1
                        completely_home_device_count                week1
                        median_home_dwell_time                      week1
                        median_non_home_dwell_time                  week1
                        median_percentage_time_home                 week1
                        population                                  week1
                        Density_Per_Sq_Mile                         week1
                        cases_adjusted_by_pop                       week1
                        new_cases_adjusted_by_pop                   week1
                        Income_level                                week1
                        stay_home_ratio                             week1
                        confirmed_cases                             week2
                        new_confirmed_cases                         week2
 


- output: the average new_confirmed_cases of the third week

Some training detail of LSTM:

- Early Stopping: when validation loss doesn't decrease, stop train.
- Save Best: we only save model with the lowest validation loss.
- Learning Rate Adjusting: when validation loss doesn't decrease for 7 epochs, reduce the learning rate.

If you find anything wrong with the code, please feel free to contact we with my email.

@University: Umich & ZJU
@author: Wenxue Li
@email: liwenxue@zju.edu.cn
@University: Umich & ZJU
@author: Xinyu Li
@email: 3170103467@zju.edu.cn

"""

import torch
import pandas as pd
import matplotlib.pyplot as plt
from sklearn import model_selection
from torch.utils.data import Dataset, DataLoader
import numpy as np
from parameter_weekly import week_input, timelagging, average_num, feature_num
from function import loss_cal, adjust_learning_rate, Net
from dataset_weekly import dataset_generate


def train_weekly():

    trainloader, train_x, train_y, validation_x, validation_y, \
    test_x, test_y, train_x_mean, train_x_std, train_y_mean, train_y_std = dataset_generate()

    """     define LSTM model   """
    model = Net(feature_num)

    """         training start!         """
    # determine optimizer, loss_function and checkpoint path
    learning_r = 0.00003
    optimizer = torch.optim.Adam(model.parameters(), lr=learning_r)
    loss_fun = torch.nn.MSELoss()
    path = 'checkpoint_2.tar'

    """ if you want train your model from a pre-trained model, uncomment the following code. """
    # checkpoint = torch.load(path)
    # model.load_state_dict(checkpoint['net'])
    # optimizer.load_state_dict(checkpoint['optimizer'])
    # validation_loss = checkpoint['best_validation_loss']
    # learining_r = 6.400000000000004e-8

    model.train()
    # val_loss_info = [validation_loss]
    val_loss_info = []      # preserve every epoch's train loss
    train_loss_info = []    # preserve every epoch's validation loss
    not_improve = 0
    for epoch in range(5000):
        for i, values in enumerate(trainloader):
            input, lable = values
            output = model(input)
            loss = loss_fun(output, lable)
            optimizer.zero_grad()
            loss.backward()
            optimizer.step()
        if epoch % 10 == 0 and epoch > 0:
            train_loss = loss_fun(model(train_x),train_y).item()
            vali_loss = loss_fun(model(validation_x), validation_y).item()
            if len(val_loss_info) == 0 or vali_loss < min(val_loss_info):  # save the best model
                not_improve = 0
                state = {'net': model.state_dict(), 'optimizer': optimizer.state_dict(),'best_validation_loss': vali_loss}
                torch.save(state, path)
                print("Saved the model.")
            else:                           # if validation loss didn't decrease, reload the best model in next epoch.
                checkpoint = torch.load(path)
                model.load_state_dict(checkpoint['net'])
                optimizer.load_state_dict(checkpoint['optimizer'])
                not_improve += 1
                print("not_improve : {}".format(not_improve))
            if (not_improve+1) % 8 == 0:    # when validation loss doesn't decrease for 7 epochs, reduce the learning rate.
                learning_r *= 0.2
                adjust_learning_rate(optimizer, learning_r)
            if (not_improve+1) % 25 == 0:   # early stopping
                print("Training End......")
                break

            print("epoch:{}, train_loss:{}, vali_loss: {}".format(epoch, train_loss, vali_loss))
            train_loss_info.append(train_loss)
            val_loss_info.append(vali_loss)

    return train_loss_info, val_loss_info
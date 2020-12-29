import torch
import pandas as pd
from sklearn import model_selection
from torch.utils.data import Dataset, DataLoader
from parameter_weekly import week_input, timelagging, average_num, feature_num


def dataset_generate():

    """    read data in and clean!     """

    filename = "zipcode_weekly_new.csv"
    zipcode_daily = pd.read_csv(filename, encoding="ISO-8859-1", dtype={'ZIP': str, 'week': str})

    zip = zipcode_daily['ZIP']  # 'ZIP' column
    del zipcode_daily['ZIP']  # delete the non-numeric columns
    del zipcode_daily['week']
    zipcode_daily = pd.DataFrame(zipcode_daily, dtype=float)  # change the type from 'int' to 'float'
    zipcode_daily['ZIP'] = zip  # add the 'ZIP' column again

    # key: 'zip code', value: feature that belong to the 'zip code'
    data_dict = {}
    for i, zipcode in enumerate(zipcode_daily[:]['ZIP']):
        if zipcode not in data_dict:
            data_dict[zipcode] = []
        feature = []
        for f in zipcode_daily.iloc[i]:
            feature.append(f)
        data_dict[zipcode].append(feature)

    train_x = []
    train_y = []
    test_x = []
    test_y = []

    ratio = 0.6  # training set ratio
    # week sequential split
    for key, values in data_dict.items():

        l = len(values)
        input_num = l - timelagging - average_num
        feature = []
        for i in range(input_num):

            index = int(input_num * ratio)

            if i <= index:  # one input point contains 2 week's data, that is week1,week2
                first = True
                for j in range(week_input):
                    if first:  # one input point contains all the feature of week1
                        for k in values[i][:-2]:
                            feature.append(k)
                        first = False
                    else:  # for week2, one input point only contains confirmed_cases & new_confirmed_cases
                        feature.append(values[i + j][0])
                        feature.append(values[i + j][1])
                train_y.append(values[i][23])  # output: average cases, that is the third week
                tmp = []
                tmp.append(feature)
                train_x.append(tmp)  # size: [1, feature_num]
                feature = []
            else:
                first = True
                for j in range(week_input):
                    if first:  # one input point contains all the feature of week1
                        for k in values[i][:-2]:
                            feature.append(k)
                        first = False
                    else:  # for week2, one input point only contains confirmed_cases & new_confirmed_cases
                        feature.append(values[i + j][0])
                        feature.append(values[i + j][1])
                test_y.append(values[i][23])  # output: average cases, that is the third week
                tmp = []
                tmp.append(feature)
                test_x.append(tmp)  # size: [1, feature_num]
            feature = []

    print(len(train_x))

    train_x_ls = []  # Change the format for later processing
    for j in train_x:
        for i in j:
            train_x_ls.append(i)
    train_x_df = pd.DataFrame(train_x_ls)
    train_y_df = pd.DataFrame(train_y)
    train_x_mean = train_x_df.mean()  # train_x dataset mean
    train_x_std = train_x_df.std()  # train_x dataset std
    train_y_mean = train_y_df.mean()  # train_y dataset mean
    train_y_std = train_y_df.std()  # train_y dataset std

    for i in range(len(train_x)):  # using train_x mean and train_x std to normalize
        for j in range(len(train_x[i])):
            for k in range(len(train_x[i][j])):
                train_x[i][j][k] = (train_x[i][j][k] - train_x_mean[k]) / train_x_std[k]

    for i in range(len(train_y)):  # using train_y mean and train_y std to normalize
        train_y[i] = (train_y[i] - train_y_mean) / train_y_std

    for i in range(len(test_x)):  # using train_x mean and train_x std to normalize
        for j in range(len(test_x[i])):
            for k in range(len(test_x[i][j])):
                test_x[i][j][k] = (test_x[i][j][k] - train_x_mean[k]) / train_x_std[k]

    for i in range(len(test_y)):  # using train_y mean and train_y std to normalize
        test_y[i] = (test_y[i] - train_y_mean) / train_y_std

    # split test to validation and test
    validation_x, test_x, validation_y, test_y = model_selection.train_test_split(test_x, test_y, test_size=0.5,
                                                                                  random_state=1)

    train_x = torch.tensor(train_x)
    train_y = torch.tensor(train_y).reshape(-1, 1)
    validation_x = torch.tensor(validation_x)
    validation_y = torch.tensor(validation_y).reshape(-1, 1)
    test_x = torch.tensor(test_x)
    test_y = torch.tensor(test_y).reshape(-1, 1)


    # define dataset
    class Mydataset(Dataset):
        def __init__(self, input, output):
            super(Mydataset, self).__init__()
            self.input = input
            self.lable = output

        def __getitem__(self, index):
            return self.input[index], self.lable[index]

        def __len__(self):
            return len(self.input)


    train_data = Mydataset(train_x, train_y)
    trainloader = DataLoader(train_data, batch_size=256, shuffle=True)

    return trainloader, train_x, train_y, validation_x, validation_y, \
           test_x, test_y, train_x_mean, train_x_std, train_y_mean, train_y_std
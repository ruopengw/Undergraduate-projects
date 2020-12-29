
"""    super parameter     """
week_input = 2                      # how many weeks' feature we use to predict
timelagging = 1                    # the length of latent window of Social Distancing data & Mobility data
average_num = 0                     # how many weeks does the average cases get from
feature_num = 23+2*(week_input-1)    # the number of feature that one input contains
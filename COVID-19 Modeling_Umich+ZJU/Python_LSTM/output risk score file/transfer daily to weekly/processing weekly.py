import pandas as pd
import math

# Read the daily data in
df_data=pd.read_csv("zipcode_daily_original.csv",index_col=False)

# Build the weekly data file
col_name = df_data.columns.tolist()
df=pd.DataFrame(columns=col_name)
del df['date']
df['ZIP'] = df_data['ZIP']
df['week'] = df_data['week']
df = df.drop_duplicates()
df.to_csv('zipcode_weekly.csv',index=False)

# Compute the weekly data for each column
df=pd.read_csv("zipcode_weekly.csv",index_col=False)
for i in range(df.shape[0]):
    day,retail,grocery,parks,transit_stations,work,residential,new_confirmed,device,complete,distance,\
    home,nonhome,new_cases_adjusted,stay_home,driving,transit,walking = 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    for j in range(df_data.shape[0]):
        # columns if ZIP codes equal, then they equal
        if (df_data.at[j,'ZIP'] == df.at[i,'ZIP']):
            df.at[i, 'Income_level'] = df_data.at[j, 'Income_level']
            df.at[i, 'population'] = df_data.at[j, 'population']
            df.at[i, 'Density_Per_Sq_Mile'] = df_data.at[j, 'Density_Per_Sq_Mile']
        # columns if dates equal the exact weeks, then they equal
        if (df_data.at[j, 'ZIP'] == df.at[i, 'ZIP']) and (df_data.at[j, 'date'] == df.at[i, 'week']):
            df.at[i, 'confirmed_cases'] = df_data.at[j, 'confirmed_cases']
            df.at[i, 'cases_adjusted_by_pop'] = df_data.at[j, 'cases_adjusted_by_pop']
        # columns for average
        if(df_data.at[j,'ZIP']==df.at[i,'ZIP']) and (df_data.at[j,'week']==df.at[i,'week']):
            day+=1 # how many days are there in a week (sometimes not 7 for the beginning)
            stay_home+=df_data.at[j,'stay_home_ratio']
            new_cases_adjusted+=df_data.at[j,'new_cases_adjusted_by_pop']
            new_confirmed+=df_data.at[j,'new_confirmed_cases']
            device+=df_data.at[j,'device_count']
            complete += df_data.at[j, 'completely_home_device_count']
            retail += df_data.at[j, 'retail_and_recreation_percent_change_from_baseline']
            grocery += df_data.at[j, 'grocery_and_pharmacy_percent_change_from_baseline']
            parks += df_data.at[j, 'parks_percent_change_from_baseline']
            transit_stations += df_data.at[j, 'transit_stations_percent_change_from_baseline']
            work += df_data.at[j, 'workplaces_percent_change_from_baseline']
            residential += df_data.at[j, 'residential_percent_change_from_baseline']
            driving += df_data.at[j,'driving']
            transit += df_data.at[j,'transit']
            walking += df_data.at[j,'walking']
            # weighted average
            distance += (df_data.at[j, 'distance_traveled_from_home'] * df_data.at[j, 'device_count'])
            home += (df_data.at[j, 'median_home_dwell_time'] * df_data.at[j, 'device_count'])
            nonhome += (df_data.at[j, 'median_non_home_dwell_time'] * df_data.at[j, 'device_count'])
    df.at[i,'new_confirmed_cases'] = new_confirmed # just add up the new cases
    df.at[i, 'new_cases_adjusted_by_pop'] = new_cases_adjusted
    df.at[i,'device_count'] = round(device/day)
    df.at[i,'completely_home_device_count'] = round(complete/day)
    df.at[i, 'retail_and_recreation_percent_change_from_baseline'] = round(retail / day)
    df.at[i, 'grocery_and_pharmacy_percent_change_from_baseline'] = round(grocery / day)
    df.at[i, 'parks_percent_change_from_baseline'] = round(parks / day)
    df.at[i, 'transit_stations_percent_change_from_baseline'] = round(transit_stations / day)
    df.at[i, 'workplaces_percent_change_from_baseline'] = round(work / day)
    df.at[i, 'residential_percent_change_from_baseline'] = round(residential / day)
    df.at[i, 'driving'] = round(driving / day)
    df.at[i, 'transit'] = round(transit / day)
    df.at[i, 'walking'] = round(walking / day)
    df.at[i, 'median_home_dwell_time'] = round(home / device)
    df.at[i, 'median_non_home_dwell_time'] = round(nonhome / device)
    df.at[i, 'median_percentage_time_home'] = round(100 * df.at[i, 'median_home_dwell_time'] / (
                df.at[i, 'median_home_dwell_time'] + df.at[i, 'median_non_home_dwell_time']))
    df.at[i, 'stay_home_ratio'] = round(100 * df.at[i, 'completely_home_device_count'] / df.at[i, 'device_count'])
    if (math.isnan(distance) == True):
        print(i)
        df.at[i, 'distance_traveled_from_home'] = 0
    else:
        df.at[i, 'distance_traveled_from_home'] = round(distance / device)
df.to_csv("zipcode_weekly.csv",index=False)

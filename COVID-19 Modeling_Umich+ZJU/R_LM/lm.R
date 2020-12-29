# ZIP code level linear model
# Author: Litian Zhou
# Date: 6/1/2020
# Conclusion here: Aborted...
# Reason: Does not consider lagging time, also daily new cases flatuated a lot,
# which make it hard to capture the pattern of spreading.


library(dplyr)
library(caret)
library(ggplot2)
library(lubridate)
library(readxl)
library(broom)

# You need to set working directory to the source file location first
zip_daily = read_excel("data/zipcode_daily_cases_social-distance_population_income.xlsx")

# Exploratory data analysis
zip_daily$date = as_date(zip_daily$date) 
zip_daily = zip_daily %>% group_by(ZIP) %>%
  arrange(date) %>%
  mutate(
    first = dplyr::first(date),
    last = dplyr::last(date)
  )

#ggplot2::ggplot(data = zip_daily, aes(x=first))+geom_bar()
#ggplot2::ggplot(data = zip_daily, aes(x=last))+geom_bar()
#ggplot2::ggplot(data = zip_daily, aes(x=date,y=confirmed_cases, color=ZIP))+geom_line()

# making new income dummy features
zip_daily$low_income = 0
zip_daily$low_income = (zip_daily$Income_level==1)
zip_daily$middle_income = 0
zip_daily$middle_income = (zip_daily$Income_level==2)
zip_daily$high_income = 0
zip_daily$high_income = (zip_daily$Income_level==3)

# add stay_home_ratio as new feature
zip_daily$stay_home_ratio = zip_daily$completely_home_device_count / zip_daily$device_count

colnames(zip_daily)[14:15] = c("cases_adjusted_by_pop","new_cases_adjusted_by_pop")

# 
# get rid of tibble to run faster and avoid session abortion
zip_daily1 = as.data.frame(zip_daily)

# Model1: previous 5 days + today's mobility to predict today's Risk(new cases_pop_adjusted）

# making previous new_confirmed_cases features
n = 5
for(j in 1:n) {
  zip_daily1[[paste0("new",j,"day_ago")]] = 0
}
# write new_n_day_ago features
for(i in 1: nrow(zip_daily1)) {
  if(as.numeric(zip_daily1[i,2]) < as.numeric(as.Date("2020-03-21")))
    next
  
  zip = as.numeric(zip_daily1[i,1])
  print(i)
  for(j in 1:n) {
    pre_date = zip_daily1[i,2] - ddays(j)
    pre_rec = zip_daily1 %>%
      filter(date == pre_date,
             ZIP == zip)
    if(nrow(pre_rec) == 0) next
    zip_daily1[[paste0("new",j,"day_ago")]][i] = pre_rec$new_confirmed_cases
  }
}

zip_daily1 = zip_daily1 %>% filter(as.numeric(date) > as.numeric(as.Date("2020-03-21")))
zip_daily_scaled1 = scale(zip_daily1[,sapply(zip_daily1, is.numeric)])
zip_daily_scaled1[,3] = zip_daily1$new_confirmed_cases
zip_daily_scaled1 = as.data.frame(zip_daily_scaled1[,-1])

set.seed(7) # set for reproducibility
train.control = trainControl(method = "cv", number = 10) # 10-fold cross validation
lm.model1 = train(data = zip_daily_scaled1, 
                  new_confirmed_cases ~.-new_cases_adjusted_by_pop,
                  method = "leapSeq",
                  tuneGrid = data.frame(nvmax = 5:15),
                  trControl = train.control
                  )

## model1 performace
lm.model1$bestTune
lm.model1$results
model1.coef = data.frame(coef = coef(lm.model1$finalModel,12),
                         features = names(coef(lm.model1$finalModel,12)))
#summary(lm.model1$finalModel)
ggplot(model1.coef, aes(x=coef, y = features)) +
  geom_point() + geom_vline(xintercept = 0, colour="red") +
  theme_minimal()

## Conlusion: low performance, not satisfactory, aborted...


# prepare new dataset with lagging effect and risk window:
# write new_n_day_after features
# n = 11
# for(j in 1:n) {
#   zip_daily2[[paste0("new",j,"day_after")]] = 0
# }
# 
# for(i in 1: nrow(zip_daily)) { # to row 10180
#   if(as.numeric(zip_daily2[i,2]) > as.numeric(as.Date("2020-05-16")))
#     next
#   
#   zip = as.numeric(zip_daily2[i,1])
#   print(i)
#   for(j in 1:n) {
#     fut_date = zip_daily2[i,2] + ddays(j)
#     fut_rec = zip_daily2 %>%
#       filter(date == fut_date,
#              ZIP == zip)
#     if(nrow(fut_rec) == 0) next
#     zip_daily2[[paste0("new",j,"day_after")]][i] = fut_rec$new_confirmed_cases
#   }
# }
# 
# zip_daily2 = zip_daily2 %>% filter(as.numeric(date) < as.numeric(as.Date("2020-05-16")))
# zip_daily2$ave_new6_9after = apply(zip_daily2[,29:32],1,mean)
# zip_daily2$ave_new7_10after = apply(zip_daily2[,30:33],1,mean)
# zip_daily2$ave_new8_11after = apply(zip_daily2[,31:34],1,mean)

# write csv file for future use
# write.csv(zip_daily2,"zipcode_daily_with_1_11_future_day_case_count.csv")



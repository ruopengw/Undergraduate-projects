# County level linear model
# Author: Litian Zhou
# Date: 6/1/2020
# Conclusion: Aborted.
# Reason: Totally not work as zip-code level. The new_n_day featuers
# not significant at all. Oddly, only the "confirmed case is significant, and
# alone contributes R-square.

library(dplyr)
library(caret)
library(ggplot2)
library(lubridate)

county_daily <- read.csv("data/Covid-19_Risk_Umich_ZJU/data/county_daily_mobility.csv")
county_daily$date = as_date(county_daily$date)
county_daily$stay_home_ratio = county_daily$completely_home_device_count/county_daily$device_count

# write new_n_day_after features
n = 10
for(j in 1:n) {
  county_daily[[paste0("new",j,"day_after")]] = 0
}

# delete NAs (delete later)
county_daily = county_daily[-c(60:73),]

for(i in 1: nrow(county_daily)) { # to row 59
  if(as.numeric(county_daily[i,1]) > as.numeric(as.Date("2020-05-16")))
    next
  
  print(i)
  for(j in 1:n) {
    fut_date = county_daily[i,1] + ddays(j)
    fut_rec = county_daily %>% filter(date == fut_date)
    if(nrow(fut_rec) == 0) next
    county_daily[[paste0("new",j,"day_after")]][i] = fut_rec$new_confirmed_cases
  }
}
county_daily = county_daily %>% filter(as.numeric(date) < as.numeric(as.Date("2020-05-04")))

county_daily$ave_new7_10after = apply(county_daily[,c("new7day_after","new8day_after",
                                                      "new9day_after","new10day_after")],1,mean)
county_daily = county_daily %>% 
  select(-c("date","new7day_after","new8day_after","new9day_after","new10day_after"))

set.seed(7)
train.control = trainControl(method = "cv", number = 10)
county_daily_scaled = scale(county_daily)
summary(test)
lm.model1 = train(data = county_daily_scaled,
                 ave_new7_10after~.,
                 method = "leapSeq",
                 tuneGrid = data.frame(nvmax=1:24),
                 trControl = train.control)

## model1 performace
lm.model1$bestTune
lm.model1$results
model1.coef = data.frame(coef = coef(lm.model1$finalModel,2),
                         features = names(coef(lm.model1$finalModel,2)))
summary(lm.model1$finalModel)
ggplot(model1.coef, aes(x=coef, y = features)) +
  geom_point() + geom_vline(xintercept = 0, colour="red") +
  theme_minimal()

# ZIP code level linear model2
# Author: Litian Zhou
# Date: 6/4/2020
# Conclusion: Adopted. 
# Reason: The R-square is 0.63 and coefficients are meaningful to draw insights.

# Model2, predict new cases b/ day t and day t+3 (risk score for risk window)
# You need to set working directory to the source file location first
zip_daily = read.csv("data/Covid-19_Risk_Umich_ZJU/data/zipcode_daily_with_1_11_future_day_case_count.csv")
zip_daily2 = as.data.frame(zip_daily)

# predict average case of the risk window
zip_daily2 = zip_daily2 %>%
  filter(as.numeric(as_date(zip_daily2$date)) < as.numeric(as_date("2020-05-16")))
zip_daily2$stay_home_ratio = zip_daily2$completely_home_device_count/zip_daily2$population
# store the scale mean and sd
scale_para = t(data.frame(mean=apply(zip_daily2[,sapply(zip_daily2, is.numeric)], 2, mean),
                          sd=apply(zip_daily2[,sapply(zip_daily2, is.numeric)], 2, sd)))
zip_daily_scaled2 = scale(zip_daily2[,sapply(zip_daily2, is.numeric)])
zip_daily_scaled2[,38] = zip_daily2$ave_new7_10after
zip_daily_scaled2 = as.data.frame(zip_daily_scaled2)
zip_daily_scaled2 = zip_daily_scaled2 %>% 
  select(-c("X","ZIP", "new7day_after","new8day_after","new9day_after", "device_count",
            "new10day_after","new11day_after","ave_new6_9after","ave_new8_11after",
            "Land_Sq_Mi","new_cases_adjusted_by_pop","cases_adjusted_by_pop","stay_home_ratio",
            "median_percentage_time_home","completely_home_device_count"))
scale_para = data.frame(scale_para) %>%
  select(-c("X","ZIP", "new7day_after","new8day_after","new9day_after", "device_count",
            "new10day_after","new11day_after","ave_new6_9after","ave_new8_11after",
            "Land_Sq_Mi","new_cases_adjusted_by_pop","cases_adjusted_by_pop","stay_home_ratio",
            "median_percentage_time_home","completely_home_device_count"))

set.seed(7) # set for reproducibility
train.control = trainControl(method = "cv", number = 10) # 10-fold cross validation
lm.model2 = train(data = zip_daily_scaled2, 
                  ave_new7_10after ~.,
                  method = "leapSeq",
                  tuneGrid = data.frame(nvmax = 3:24),
                  trControl = train.control
)

## model2 performace
lm.model2$bestTune
lm.model2$results 
lm.model2
intercept = coef(lm.model2$finalModel,as.numeric(lm.model2$bestTune))[1]
coef_scaled = coef(lm.model2$finalModel,as.numeric(lm.model2$bestTune))[-1]
coef_name = names(coef(lm.model2$finalModel,as.numeric(lm.model2$bestTune)))[-1]
coef_ori_scale = as.numeric(coef_scaled * scale_para[2,coef_name] + scale_para[1,coef_name])
model2.coef = data.frame(cbind(round(coef_scaled,3),round(coef_ori_scale)))
colnames(model2.coef) = c("coefficients with standardized predictors", "coefficients of original scale")
#write.csv(model2.coef, "lm_coefficients.csv")    #show on the report and website


models <- zip_daily_scaled2 %>% 
  select(c(ave_new7_10after,coef_name[-3])) %>%
  do(data.frame(tidy(lm(ave_new7_10after ~ ., data = .),conf.int=T )))

models$term[10:12] = c("retail_visit","grocery_pharmacy_visit","parks_visit")
models = models[c(12:7,6:3,18:13,3:1),]

ggplot(models, aes(x=estimate, y = term)) +
  geom_point() + geom_vline(xintercept = 0, colour="red") +
  geom_errorbar(aes(xmin=conf.low, xmax=conf.high), size=0.1, width = 0.3) +
  scale_y_discrete(limits=models$term)+
  theme_minimal() + xlab("coefficient estimate") + ylab("predictor") +
  theme(axis.text = element_text(size=rel(1.2)))

ggplot(varImp(lm.model2), aes(x=Overall)) + geom_col() +
  theme_minimal() + xlab("Predictor") +
  theme(axis.text.y=element_text(size =rel(1.2)))

# features order: mobility, case count, social economic
y_yhat = data.frame(predict_case=predict(lm.model2), true_case=zip_daily_scaled2$ave_new7_10after)
ggplot(y_yhat, aes(x=predict_case,y=true_case)) +
  geom_point() + geom_abline(slope = 1, intercept = 0, color="red") +
  coord_fixed(ratio = 1) +
  theme_minimal()
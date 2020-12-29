function [EFF_T,EFF_R] = RCWA_user(period,thickness,dutycycle)%grating shape not included
    EFF_T = [];
    EFF_R = [];
    load('TiO2.mat');
    addpath('/Users/wrp/浙江大学/暑研/Umich/ricwaa 1.0.2');
    for wavelength=400:10:680 % parfor must be continuous
            n_grating = interp1(WL, R, wavelength)+1i*interp1(WL, I, wavelength);
            [eff_t,eff_r] = RCWA_recall(wavelength,period,thickness,dutycycle,n_grating);
            EFF_T = [EFF_T, eff_t];
            EFF_R = [EFF_R, eff_r];
    end
    wavelength=400:10:680;
    plot(wavelength, EFF_T);
end
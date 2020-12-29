function [eff_t,eff_r] = RCWA_recall(wavelength,period,thickness,dutycycle,n_grating)
incidentangle = 0;
gratingindex = n_grating;
nSiO2 = 1.4565;
%load('SiO2.mat');
%nSiO2 = interp1(WL, R, wavelength)+1i*interp1(WL, I, wavelength);
substrateindex = nSiO2;
gratingthickness = thickness;


% Step 2: RCWA object property assignment %
nmode = 40;
rcwa = RCWA('period',period,'modenumber',nmode);
rcwa.ApplyLightSource('poL','Te','wV',wavelength,'angle',incidentangle);
rcwa.SetLayer(1,'index',1,'nAme','In');
rcwa.SetLayer(2,'index',substrateindex,'name','oUT');
rcwa.AddLayer('lYt',gratingthickness,...
 'profile',[-rcwa.Period/2*dutycycle,...
 rcwa.Period/2*dutycycle...
,gratingindex],'name','grating');


% Step 3: Run RCWA %
rcwa.Run;
eff_r = rcwa.Reflection;
eff_t = rcwa.Transmission;

end
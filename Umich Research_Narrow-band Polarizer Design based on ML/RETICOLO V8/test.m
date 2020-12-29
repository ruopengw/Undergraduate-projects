%%%%%%%%%%%%%%%%%%%%%%%%%
% EXAMPLE 1D (TE or TM) %
%%%%%%%%%%%%%%%%%%%%%%%%%
wavelength=5;
period=10;% same unit as wavelength
n_incident_medium=1;% refractive index of the top layer
n_transmitted_medium=1.5;% refractive index of the bottom layer
angle_theta0=0;k_parallel=n_incident_medium*sin(angle_theta0*pi/180);
parm=res0(1);% TE polarization. For TM : parm=res0(-1)
parm.res1.champ=1;% the electromagnetic field is calculated accurately
nn=40;% Fourier harmonics run from [-40,40]
% textures for all layers including the top and bottom layers
texture=cell(1,3);
textures{1}= n_incident_medium;                   % uniform texture
textures{2}= n_transmitted_medium;                % uniform texture

textures{3}={[-2.5,2.5],[n_incident_medium,n_transmitted_medium] };
aa=res1(wavelength,period,textures,nn,k_parallel,parm);
profile={[4.1,5.2,4.1],[1,3,2]};
one_D_TE=res2(aa,profile)
eff=one_D_TE.inc_top_reflected.efficiency{-1}
J=one_D_TE.Jones.inc_top_reflected{-1};% Jones’coefficients
abs(J)^2 % first order efficiency for an illumination from the top layer
% field calculation
x=linspace(-period/2,period/2,51);% x coordinates(z-coordinates are determined by res3.m)
einc=1;
parm.res3.trace=1; % plotting automatically
parm.res3.npts=[50,50,50];
[e,z,index]=res3(x,aa,profile,einc,parm);
figure;pcolor(x,z,real(squeeze(e(:,:,1)))); % user plotting
shading flat;xlabel('x');ylabel('y');axis equal;title('Real(Ey)');
% Loss calculation
textures{3}={[-2.5,2.5],[n_incident_medium,.1+5i] };
aa_loss=res1(wavelength,period,textures,nn,k_parallel,parm);
one_D_loss=res2(aa_loss,profile)
parm.res3.npts=[[0,10,0];[1,3,1]];
einc=one_D_loss.inc_top.PlaneWave_E(2);
[e,z,index,wZ,loss_per_layer,loss_of_Z,loss_of_Z_X,X,wX]=res3([-period/2,period/2],aa_loss,profile,einc,parm);
Energie_conservation=sum(one_D_loss.inc_top_reflected.efficiency)+sum(one_D_loss.inc_top_transmitted.efficiency)+sum(loss_per_layer)/(.5* period)-1
retio % erase temporary files
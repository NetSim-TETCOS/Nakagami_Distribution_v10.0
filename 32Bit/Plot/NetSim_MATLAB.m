function WLAN=NetSim_MATLAB(choice,varargin)
 
switch(choice)
 
case 'nakagami'

h=ProbDistUnivParam('nakagami',[varargin{1},varargin{2}]);
i=random(h,1);
fid = fopen('plotvalues.txt','a+');
fprintf(fid,'%f',i);
fprintf(fid,'\r\n');
fclose('all');
WLAN=i;
 
case 'plothistogram'
  
fid=fopen('plotvalues.txt');
mx=fscanf(fid,'%f');
hist(mx);
fclose('all');
 
end

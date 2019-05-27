function [data] = read_imu_to_workspace()
raw = csvread('data.txt');

data = array2table(raw,...
    'VariableNames',{'dT','ax','ay','az','gx','gy','gz','phi','theta'});
end


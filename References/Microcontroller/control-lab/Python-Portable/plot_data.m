clear all;
close all;

data10mS = read_control_to_workspace('data10mS.txt');
t10mS = 0:0.010:(size(data10mS.y) -1) * 0.010;

data25mS = read_control_to_workspace('data25mS.txt');
t25mS = 0:0.010:(size(data25mS.y) -1) * 0.010;

figure()
plot(t10mS, data10mS.r, t10mS, data10mS.y);grid

figure()
plot(t25mS, data25mS.r, t25mS, data25mS.y);grid
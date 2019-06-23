clear all;
close all;

data10mS = read_control_to_workspace('data.txt');
t10mS = 0:0.010:(size(data10mS.y) -1) * 0.010;


figure()
plot(t10mS, data10mS.r, t10mS, data10mS.y);grid

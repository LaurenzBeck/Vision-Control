function [data] = read_control_to_workspace(path)
raw = csvread(path);

data = array2table(raw,...
    'VariableNames',{'y','r','e','u'});
end


data = csvread('basic_pid_controller_closed_loop_system.csv');
t = data(:,1);

figure;
hold on;
plot(t, data(:,2), ";set point;");
plot(t, data(:,3), ";controller feedback input;");
% plot(t, data(:,4), ";controller error;");
% plot(t, data(:,5), ";controller error (integral);");
% plot(t, data(:,6), ";controller error (differential);");
plot(t, data(:,7), ";controller output;");
hold off
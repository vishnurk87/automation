rng(0,'twister'); % code to fix a set of random vertices

no_point = 10;
% x = randi([-20 20],1,no_point);
% y = randi([-20 20],1, no_point);
x = [0 10 60 40 90 140 180 1600 50 10];
y = [0 10 80 100 50 90 130 78 60 6];
% plotting points

axis equal;
p1 = plot(x,y, 'o k');
xlabel('x axis');
ylabel('y axis');
title('Curves');
hold on;

% axis([-20 20 -20 20]);
display(x);
display(y);
n = no_point-1;

% part 2, Bezier curve
% initialising vectors
ax = zeros(1,no_point);
ay = zeros(1,no_point);
X = zeros(1,50);
Y = zeros(1,50);
u = linspace(0,1,50);

% loop of equations of blending function

for j = 1:50;
    for i = 0:n
        ax(i+1) = (factorial(n)/(factorial(i)*factorial(n-i)))*((u(j))^i)*((1-u(j))^(n-i))*x(i+1);
        ay(i+1) = (factorial(n)/(factorial(i)*factorial(n-i)))*((u(j))^i)*((1-u(j))^(n-i))*y(i+1);
    end
    X(j) = sum(ax);
    Y(j) = sum(ay);  
end
% 
% %plotting the Bezier curve
p2 = plot(X,Y, 'b');

fileID = fopen('output.txt','w');
fprintf(fileID,'%f %f\n',round(X,2),round(Y,2));
fclose(fileID);
hold off;
% Mech 6451/X Computer-Assisted Mechanical design
% Assignment 3 
% By Vishnu Radhakrishnan 
% student Id: 40118055

% 1) generation of random points withing (-20 20) interval number of
% vertices = no_point
rng(0,'twister'); % code to fix a set of random vertices

no_point = 10;
% x = randi([-20 20],1,no_point);
% y = randi([-20 20],1, no_point);
x = [0 10 60 40 90 140 180 160 50 10];
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

%plotting the Bezier curve
% p2 = plot(X,Y, 'b');



% 3) B-Spline:  Non uniform Non Periodic
% order = k = 4
k = 4;

% loop to find knot values(non uniform)
t = zeros();
for i = 0:k-1;
    t(i+1) = 0;
end
for i = k:n;
    t(i+1) = i - k + 1.5;   % values changed from 1 to 1.5 for non uniformity
end
for i = n+1:n+k;
    t(i+1) = n - k + 2.6;     % values changed from 2 to 3 for non uniformity
end
t(k+1) = 1.5;     % additional additions for non uniformity
display(t);
N = zeros();
X1 = zeros();
Y1 = zeros();

% defining parameter (u) values

U = linspace(0,t(end),50);
for u = 1:50
    for i = 1:size(t,2)-1
        if t(i+1) >= U(u) && t(i) <= U(u) && t(i+1) - t(i) > 0;
            N(1,i) = 1;
            
        else 
            N(1,i) = 0;
        end
    end
    for K = 2:k
        
        for i = 1:size(t,2)-2
            
             if (i+K) <= size(t,2) 
                a = (((U(u) - t(i))*N((K-1),i))/(t(i+K-1)-t(i)));
               
                b = (((t(i+K) - U(u))*N((K-1),i+1))/(t(i+K) - t(i+1)));
                
                a(isnan(a)) = 0; % changing zero division and infinity values to zero
                b(isnan(b)) = 0;
                a(isinf(a)) = 0;
                b(isinf(b)) = 0;
                N(K,i) = a+b;
                N(isnan(N))=0;
            end
        end   
    end 

    for i = 1:size(x,2)
        X1(u,i) = N(k,i)*x(i);
        Y1(u,i) = N(k,i)*y(i);
    end
      
end
X = sum(X1,2);
Y = sum(Y1,2);

% plotting B-Spline Curve
% p3 = plot(X,Y, 'g');

% 4 NURBS
% order of NURBS k = 4
k = 4;

%random number generation for hi between 0.2 and 5
gg = 0.2;
hh = 5.0;
h = (hh-gg).*rand(1,no_point) + gg;

N = zeros();
X1 = zeros();
Y1 = zeros();
h1 = zeros();
clearvars X Y U a b u;
U = linspace(0,t(end),1000);
for u = 1:1000
    for i = 1:size(t,2)-1
        if t(i+1) >= U(u) && t(i) <= U(u) && t(i+1) - t(i) > 0;
            N(1,i) = 1;
            
        else 
            N(1,i) = 0;
        end
    end
    for K = 2:k
        
        for i = 1:size(t,2)-2
            
             if (i+K) <= size(t,2) 
                a = (((U(u) - t(i))*N((K-1),i))/(t(i+K-1)-t(i)));
               
                b = (((t(i+K) - U(u))*N((K-1),i+1))/(t(i+K) - t(i+1)));
                a(isnan(a)) = 0; % changing zero division and infinity values to zero
                b(isnan(b)) = 0;
                a(isinf(a)) = 0;
                b(isinf(b)) = 0;
                N(K,i) = a+b;
                N(isnan(N))=0;
            end
        end   
    end 
    %%
    
%%
    for i = 1:size(x,2)
        X1(u,i) = N(k,i)*(dot(x(i),h(i)));
        Y1(u,i) = N(k,i)*(dot(y(i),h(i)));
        h1(u,i) = N(k,i)*h(i);
    end
      
end
X = sum(X1,2)./sum(h1,2);
Y = sum(Y1,2)./sum(h1,2);
display(h); 

% plotting NURBS curve
p4 = plot(X,Y,'r');

% txt = ['knot values: ' num2str(t(1,:)), 'FontSize',1];
% text(0,0.5,txt)
fileID = fopen('outputx.txt','w');
fprintf(fileID,'%f\n',X);
fclose(fileID);
hold off;
fileID = fopen('outputy.txt','w');
fprintf(fileID,'%f\n',Y);
fclose(fileID);
hold off;
% legend([p1 p2 p3 p4],{'Control Vertices', 'Bezier Curve', 'B-Spline Curve', 'NURBS'}, 'FontSize',8);

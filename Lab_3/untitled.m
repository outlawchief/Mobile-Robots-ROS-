fileID = fopen('simdata.txt','r');
count = 1;
data = zeros(200, 65);
%while ~feof(fileID)
for k = 1:200
    str = fscanf(fileID,'%s',1);
  
    C = strsplit(str,',');
    A = cell2mat(C);
    %display(A(1))
    %display(size(C{1}));
    
    for n = 1:65
        x = 2; 
        x= str2double(C{n});
        if str2double(C{n}) > 5
            x = str2double(C{n}) - 18;
        end
        
        data(count,n) = x;
    end
    count = count +1;
    
 
    
end



   [n,p] = size(data);
    t = 1:65;
    display(size(data(1,:)))
    plot(t,data(50,:))

fclose(fileID);
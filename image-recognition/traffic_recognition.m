% traffic_recognition.m
%	Giannhs Apostolidhs, giannhs.apostolidhs@gmail.com
%	03/06/2013
%

clear all;

CAR_DIRECTION_DOWN = 1;
CAR_DIRECTION_UP = 2;

DIRECTION = CAR_DIRECTION_DOWN;

STATE_NEW_ENTRY = 1;
STATE_REFRESH = 2;
STATE_NOT_FIND = 3;
STATE_ZOMBIE = 4;
STATE_DELETE = 10;

FRAMES_TO_COUNT_CAR = 3;

videoName = 'traffic'; % videos\custom_video_1 videos\custom_video_2 videos\youtube_video
videoFormat = 'avi';

videoRealName = strcat(videoName, '.', videoFormat);
videoStoreName = strcat(videoName, '_rec', '.', videoFormat);

trafficObj = VideoReader( videoRealName ); %done

nframes = get(trafficObj, 'NumberOfFrames');
frameRate = get(trafficObj,'FrameRate');

writerObj = VideoWriter( videoStoreName );   

writerObj.FrameRate = frameRate;
open(writerObj);
I = read(trafficObj, 1);
taggedCars = zeros([size(I,1) size(I,2) 3 nframes], class(I));

BOX_SIZE_X = trafficObj.Width/19;
BOX_SIZE_Y = trafficObj.Width/12;

CROPPED_Y = trafficObj.Height*0.3;
CROPPED_X = trafficObj.Width*0.15;
CROPPED_HEIGHT = trafficObj.Height - CROPPED_Y*2;
CROPPED_WIDTH = trafficObj.Width - CROPPED_X*2;
    
CAR_ACCEPT_HEIGHT = CROPPED_HEIGHT/4;
se = strel('disk',6);
carsCounter=0;
cars = [];

for k = 1 : nframes-1
    currFrame = read(trafficObj, k);
    nextFrame = read(trafficObj, k + 1);
    
    taggedCars(:,:,:,k) = currFrame;
    
    currFrameGray = rgb2gray(currFrame);
    nextFrameGray = rgb2gray(nextFrame);

    currFrameEdges = edge(currFrameGray, 'sobel');
    nextFrameEdges = edge(nextFrameGray, 'sobel');
    
    motionVector = abs(currFrameEdges-nextFrameEdges);
   
    motionVectorbw = imcrop(motionVector, [CROPPED_X, CROPPED_Y, CROPPED_WIDTH, CROPPED_HEIGHT]);

    motionVectorbw = bwareaopen(motionVectorbw, 25);
    
    motionVectorbw = imfill(motionVectorbw,'holes');
    motionVectorbw = imclose(motionVectorbw,se);
    motionVectorbw = imfill(motionVectorbw,'holes');
    
    stats = regionprops(motionVectorbw, 'Centroid');
    
    for i = 1:length(cars)
        if cars{i}.mode<STATE_ZOMBIE
            cars{i}.mode = STATE_NOT_FIND; 
        end 
    end

    for i = 1:length(stats)
        stat = stats(i);
        objectExist = false;
        for j = 1:length(cars)
            A = [stat.Centroid BOX_SIZE_X BOX_SIZE_Y];
            B = [cars{j}.Centroid BOX_SIZE_X BOX_SIZE_Y];
            d = rectint(A,B);
            if cars{j}.mode>=STATE_ZOMBIE
                if d~=0
                    cars{j}.Centroid = stat.Centroid;
                    objectExist = true;
                end 
            elseif d~=0
                cars{j}.Centroid = stat.Centroid;
                cars{j}.mode = STATE_REFRESH;
                cars{j}.cnt = cars{j}.cnt + 1;
                objectExist = true;
                break;
            end
        end 
        objectOnPassingLine = false;
        objectOnPassingLineBool = false;
        if DIRECTION==CAR_DIRECTION_UP
            objectOnPassingLineBool = stat.Centroid(2)>=CAR_ACCEPT_HEIGHT*2 && stat.Centroid(2)<=trafficObj.Height;
        else
            objectOnPassingLineBool = stat.Centroid(2)>=0 && stat.Centroid(2)<= CAR_ACCEPT_HEIGHT;
        end
        if objectOnPassingLineBool
            objectOnPassingLine = true; 
        end
               
        if objectExist==false && objectOnPassingLine==true
            temp = {};
            temp.Centroid = stat.Centroid;
            temp.mode = STATE_NEW_ENTRY;
            temp.cnt = 1;
            cars{end+1} = temp;
        end  
    end
    
     for i = 1:length(stats)
        row = uint16(stats(i).Centroid(2)) + CROPPED_Y;
        col = uint16(stats(i).Centroid(1)) + CROPPED_X;

        row = uint16(max(row-BOX_SIZE_Y*0.6, 1):min(row+BOX_SIZE_Y*0.6, size(I,1)));
        col = uint16(max(col-BOX_SIZE_X*0.6, 1):min(col+BOX_SIZE_X*0.6, size(I,2)));

        color = (i/length(cars))*255;
        taggedCars(row,col,1,k) = taggedCars(row,col,2,k)/2;
        taggedCars(row,col,2,k) = color;
        taggedCars(row,col,3,k) = taggedCars(row,col,2,k)/2; 
    end  
    
    for i = 1:length(cars)
        row = uint16(cars{i}.Centroid(2)) + CROPPED_Y;
        col = uint16(cars{i}.Centroid(1)) + CROPPED_X;

        row = uint16(max(row-BOX_SIZE_Y, 1):min(row+BOX_SIZE_Y, size(I,1)));
        col = uint16(max(col-BOX_SIZE_X, 1):min(col+BOX_SIZE_X, size(I,2)));

        color = (i/length(cars))*255;
        if cars{i}.mode<STATE_ZOMBIE
            taggedCars(row,col,1,k) = color;
            taggedCars(row,col,2,k) = taggedCars(row,col,2,k)/2;
            taggedCars(row,col,3,k) = taggedCars(row,col,2,k)/2; 
        else
            taggedCars(row,col,1,k) = taggedCars(row,col,2,k)/1.5;
            taggedCars(row,col,2,k) = taggedCars(row,col,2,k)/1.5;
            taggedCars(row,col,3,k) = color; 
        end
    end 
        
    I=taggedCars(:,:,:,k);
    imshow(I), hold on 
    
    i=1;
    while(i<=length(cars))
        if cars{i}.mode>=STATE_ZOMBIE
            cars{i}.mode = cars{i}.mode+1;
            if cars{i}.mode==STATE_DELETE
                cars(i) = [];
            else
                i=i+1;
            end
        else
            if cars{i}.mode==STATE_NOT_FIND
                if cars{i}.cnt == -2;
                    cars{i}.mode = STATE_ZOMBIE;
                elseif cars{i}.cnt==-1;
                    cars{i}.cnt=-2
                else
                    cars{i}.cnt = -1;
                end 
            elseif cars{i}.cnt == FRAMES_TO_COUNT_CAR
                text(cars{i}.Centroid(1)+ CROPPED_X, cars{i}.Centroid(2)+ CROPPED_Y ,num2str(carsCounter+1),'Color',[0,0,0],'FontSize',20);
                cars{i}.mode = STATE_ZOMBIE;
                carsCounter = carsCounter +1;
            end    
            i=i+1;
        end
        
    end
     
    text(10,CROPPED_Y+CROPPED_HEIGHT,num2str(carsCounter),'Color','white','FontWeight','Bold','FontSize',20);
    printedFrame = getframe;
    taggedCars(:,:,:,k)=printedFrame.cdata([1:trafficObj.Height], [1:trafficObj.Width], :);

    press = uint8((k/(nframes-1)) * 100);
    if mod(press,5)==0
       fprintf('editing... %d%%\n', press);
    end
    
    writeVideo(writerObj,taggedCars(:,:,:,k));
end

fprintf('\nediting done.\nProcessed video is stored as "%s".', videoStoreName);
close(writerObj);

implay(taggedCars,frameRate);
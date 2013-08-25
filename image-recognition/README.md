#Tracking cars in video sequences

## We implement a traffic recognition system. 
## We create an algorithm separated in two parts

1. Image analysis, finding and bold noticeable motion between the current and next frame. Try to reconstruct the car shape from the motion vector. independently to the video sequence.
2. Car recognition and counting algorithm, We develop an algorithm that is getting the analyzed image as input and trying to recognize unique every car at the depth of frame and depose things that are not cars like trees or people motion. It is dependent to the video sequence.
#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>


using namespace std;
using namespace cv;

static Mat image;
static Rect2d boundingBox;
static bool paused;
static bool selectObject = false;
static bool startSelection = false;

static const char* keys =
{ "{@tracker_algorithm | | tracker algorithm }"
    "{@video_name        | | video name        }" };

static void help()
{
  cout << "\nThis example shows the functionality of \"Long-term optical tracking API\""
       "-- pause video [p] and draw a bounding box around the target to start the tracker\n"
       "Call:\n"
       "./tracker <tracker_algorithm> <video_name>\n"
       << endl;

  cout << "\n\nHot keys: \n"
       "\tq - quit the program\n"
       "\tp - pause video\n";
}

static void onMouse( int event, int x, int y, int, void* )
{
  if( !selectObject )
  {
    switch ( event )
    {
      case EVENT_LBUTTONDOWN:
        //set origin of the bounding box
        startSelection = true;
        boundingBox.x = x;
        boundingBox.y = y;
        break;
      case EVENT_LBUTTONUP:
        //sei with and height of the bounding box
        boundingBox.width = std::abs( x - boundingBox.x );
        boundingBox.height = std::abs( y - boundingBox.y );
        cout << boundingBox.x << endl;
        cout << boundingBox.y << endl;
        cout << boundingBox.width << endl;
        cout << boundingBox.height << endl;
        selectObject = true;
        break;
      case EVENT_MOUSEMOVE:

        if( startSelection && !selectObject )
        {
          //draw the bounding box
          Mat currentFrame;
          image.copyTo( currentFrame );
          rectangle( currentFrame, Point( boundingBox.x, boundingBox.y ), Point( x, y ), Scalar( 255, 0, 0 ), 2, 1 );
          imshow( "Tracking API", currentFrame );
        }
        break;
    }
  }
}

int main( int argc, char** argv )
{
  //String video_name = parser.get<String>( 1 );

  //open the capture
  VideoCapture cap;
  cap.open( "8Meters_TightLeft.avi" );
  TickMeter t;
  string j;

  if( !cap.isOpened() )
  {
    help();
    cout << "***Could not initialize capturing...***\n";
    cout << "Current parameter's value: \n";
    return -1;
  }

  Mat frame;
  paused = false;
  namedWindow( "Tracking API", 1 );
  setMouseCallback( "Tracking API", onMouse, 0 );

  //instantiates the specific Tracker
  std::cout << cv::getBuildInformation() << std::endl;
  Ptr<TrackerGOTURN> tracker = TrackerGOTURN::create();
  if( tracker == NULL )
  {
    cout << "***Error in the instantiation of the tracker...***\n";
    return -1;
  }

  //get the first frame
  cap >> frame;
  cap >> frame;
  frame.copyTo( image );
  imshow( "Tracking API", image );

  bool initialized = false;
  int i = 0;
  for (;;)
  {
    if( !paused )
    {
      i++;
      cout << i << endl;
       // if (selectObject) paused = true;
      cap >> frame;
      frame.copyTo( image );
      if (i == 176) {
            paused = true;

            boundingBox.x = 270;
            boundingBox.y = 262;
            boundingBox.width = 14;
            boundingBox.height = 14;
            selectObject = true;

            rectangle( image, boundingBox, Scalar( 255, 0, 0 ), 2, 1 );
        }
        if (selectObject) paused = true;

      if( !initialized && selectObject )
      {
        //initializes the tracker
        if( !tracker->init( frame, boundingBox ) )
        {
          cout << "***Could not initialize tracker...***\n";
          return -1;
        }
        initialized = true;
      }
      else if( initialized )
      {
        //updates the tracker
        if( tracker->update( frame, boundingBox ) )
        {
          rectangle( image, boundingBox, Scalar( 255, 0, 0 ), 2, 1 );
        }
      }
      imshow( "Tracking API", image );
    }

    char c = (char) waitKey( 2 );
    if( c == 'q' )
      break;
    if( c == 'p' )
      paused = !paused;

  }

  return 0;
}

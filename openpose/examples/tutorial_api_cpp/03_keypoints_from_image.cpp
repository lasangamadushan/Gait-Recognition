// ----------------------- OpenPose C++ API Tutorial - Example 3 - Body from image -----------------------
// It reads an image, process it, and displays it with the pose (and optionally hand and face) keypoints. In addition,
// it includes all the OpenPose configuration flags (enable/disable hand, face, output saving, etc.).
//#include "boost/filesystem.hpp"
#include <windows.h>
// Third-party dependencies
#include <opencv2/opencv.hpp>
// Command-line user interface
#define OPENPOSE_FLAGS_DISABLE_PRODUCER
#define OPENPOSE_FLAGS_DISABLE_DISPLAY
#include <openpose/flags.hpp>
// OpenPose dependencies
#include <openpose/headers.hpp>
#include <openpose/gui/guiInfoAdder.hpp>

// Custom OpenPose flags
// Producer
DEFINE_string(image_path, "examples/media/COCO_val2014_000000000294.jpg",
    "Process an image. Read all standard formats (jpg, png, bmp, etc.).");
// Display
DEFINE_bool(no_display,                 false,
    "Enable to disable the visual display.");

// This worker will just read and return all the jpg files in a directory
void display(const std::shared_ptr<std::vector<std::shared_ptr<op::Datum>>>& datumsPtr)
{
    try
    {
        // User's displaying/saving/other processing here
            // datum.cvOutputData: rendered frame with pose or heatmaps
            // datum.poseKeypoints: Array<float> with the estimated pose
		/*cv::Mat atom_image = cv:: Mat::zeros(350, 350, CV_8UC3);
		for (int i = 0; i < 25; i+=3) {
			cv::line(atom_image, cv::Point(datumsPtr->at(0)->poseKeypoints.at(i), datumsPtr->at(0)->poseKeypoints.at(i+1)),
				cv::Point(datumsPtr->at(0)->poseKeypoints.at(i), datumsPtr->at(0)->poseKeypoints.at(i+1)), 255, 5, 8, 0);
		}

		std::cout << "point is " << datumsPtr->at(0)->poseKeypoints.at(0) << std::endl;
		std::cout << "point is " << datumsPtr->at(0)->poseKeypoints.at(1) << std::endl;
		cv::imshow("new image",atom_image);
		*/
        if (datumsPtr != nullptr && !datumsPtr->empty())
        {
            // Display image
            const cv::Mat cvMat = OP_OP2CVCONSTMAT(datumsPtr->at(0)->cvOutputData);
            cv::imshow(OPEN_POSE_NAME_AND_VERSION + " - Tutorial C++ API", cvMat);
            cv::waitKey(0);
        }
        else
            op::opLog("Nullptr or empty datumsPtr found.", op::Priority::High);
    }
    catch (const std::exception& e)
    {
        op::error(e.what(), __LINE__, __FUNCTION__, __FILE__);
    }
}

void printKeypoints(const std::shared_ptr<std::vector<std::shared_ptr<op::Datum>>>& datumsPtr)
{
    try
    {
        // Example: How to use the pose keypoints
        if (datumsPtr != nullptr && !datumsPtr->empty())
        {
            op::opLog("Body keypoints: " + datumsPtr->at(0)->poseKeypoints.toString(), op::Priority::High);
            op::opLog("Face keypoints: " + datumsPtr->at(0)->faceKeypoints.toString(), op::Priority::High);
            op::opLog("Left hand keypoints: " + datumsPtr->at(0)->handKeypoints[0].toString(), op::Priority::High);
            op::opLog("Right hand keypoints: " + datumsPtr->at(0)->handKeypoints[1].toString(), op::Priority::High);
        }
        else
            op::opLog("Nullptr or empty datumsPtr found.", op::Priority::High);
    }
    catch (const std::exception& e)
    {
        op::error(e.what(), __LINE__, __FUNCTION__, __FILE__);
    }
}

bool validateDatum(const std::shared_ptr<std::vector<std::shared_ptr<op::Datum>>>& datumsPtr) {
	int noneZeroCount = 0;
	int max = -1;
	for (int i = 0; i < datumsPtr->at(0)->poseKeypoints.getVolume(); i++) {
		if (datumsPtr->at(0)->poseKeypoints.at(i) != 0) {
			noneZeroCount++;
		}
		if (i % 3 == 0) {
			if (max < datumsPtr->at(0)->poseKeypoints.at(i)) {
				max = datumsPtr->at(0)->poseKeypoints.at(i);
			}
		}
	}
	
	std::cout << "None Zero Count: " << noneZeroCount << std::endl;

	if (noneZeroCount > 45) {
		if (max < 1200) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

int cgetWidth(const std::shared_ptr<std::vector<std::shared_ptr<op::Datum>>>& datumsPtr) {
	std::cout << "get width, array size: " << datumsPtr->at(0)->poseKeypoints.getVolume() << std::endl;
	int max = -1;
	int min = 10000;
	if (datumsPtr->at(0)->poseKeypoints.getVolume() > 2) {
		for (int i = 0; i < datumsPtr->at(0)->poseKeypoints.getVolume() / 3; i++) {
			if (max < datumsPtr->at(0)->poseKeypoints.at((i * 3) )) {
				max = datumsPtr->at(0)->poseKeypoints.at((i * 3) );
			}
			if (min > datumsPtr->at(0)->poseKeypoints.at((i * 3) ) && datumsPtr->at(0)->poseKeypoints.at((i * 3)) !=0) {
				min = datumsPtr->at(0)->poseKeypoints.at((i * 3) );
			}
		}

		std::cout << "width: " << max - min << std::endl;
		return (max - min);
	}
	else {
		return 0;
	}
}

int getWidth(const std::shared_ptr<std::vector<std::shared_ptr<op::Datum>>>& datumsPtr) {
	std::cout << "get width, array size: " << datumsPtr->at(0)->poseKeypoints.getVolume() << std::endl;
	int lowH, lowL =0;
	int maxH = -1;
	int	maxL = -1;
	if (datumsPtr->at(0)->poseKeypoints.getVolume() > 2) {
		for (int i = 0; i < datumsPtr->at(0)->poseKeypoints.getVolume() / 3; i++) {
			if (maxL < datumsPtr->at(0)->poseKeypoints.at((i * 3)+1)) {
				maxL = datumsPtr->at(0)->poseKeypoints.at((i * 3)+1);
				lowL = (i * 3);
				if (maxL > maxH) {
					int temp = maxH;
					maxH = maxL;
					maxL = temp;

					temp = lowH;
					lowH = lowL;
					lowL = temp;
				}
			}
		}
		int width = datumsPtr->at(0)->poseKeypoints.at(lowL) - datumsPtr->at(0)->poseKeypoints.at(lowH);
		std::cout << "lowest points : " << maxH << "  " << maxL << std::endl;
		std::cout << "width: " << std::abs(width) << std::endl;
		const cv::Mat cvMat = OP_OP2CVCONSTMAT(datumsPtr->at(0)->cvOutputData);
		//op::opLog("Body keypoints: " + datumsPtr->at(0)->poseKeypoints.toString(), op::Priority::High);
		/*cv::circle(cvMat, cv::Point(datumsPtr->at(0)->poseKeypoints.at(lowL), datumsPtr->at(0)->poseKeypoints.at(lowL + 1)), 30, cv::Scalar(255, 0, 0), 10);
		cv::circle(cvMat, cv::Point(datumsPtr->at(0)->poseKeypoints.at(lowH), datumsPtr->at(0)->poseKeypoints.at(lowH + 1)), 30, cv::Scalar(255, 0, 0), 10);
		cv::imshow(OPEN_POSE_NAME_AND_VERSION + " - Foot Points", cvMat);
		cv::waitKey(1);*/
		return std::abs(width);
	}
	else {
		return 0;
	}
}

int prevWidth = NULL;
int getDiff(const std::shared_ptr<std::vector<std::shared_ptr<op::Datum>>>& current) {
	std::cout << "get diff" << std::endl;
	if (getWidth(current) - prevWidth > 10) {
		return 1;
	}
	else if (getWidth(current) - prevWidth < -10) {
		return -1;
	}
	else {
		return 0;
	}
}

void configureWrapper(op::Wrapper& opWrapper)
{
    try
    {
        // Configuring OpenPose

        // logging_level
        op::checkBool(
            0 <= FLAGS_logging_level && FLAGS_logging_level <= 255, "Wrong logging_level value.",
            __LINE__, __FUNCTION__, __FILE__);
        op::ConfigureLog::setPriorityThreshold((op::Priority)FLAGS_logging_level);
        op::Profiler::setDefaultX(FLAGS_profile_speed);

        // Applying user defined configuration - GFlags to program variables
        // outputSize
        const auto outputSize = op::flagsToPoint(op::String(FLAGS_output_resolution), "-1x-1");
        // netInputSize
        const auto netInputSize = op::flagsToPoint(op::String(FLAGS_net_resolution), "-1x368");
        // faceNetInputSize
        const auto faceNetInputSize = op::flagsToPoint(op::String(FLAGS_face_net_resolution), "368x368 (multiples of 16)");
        // handNetInputSize
        const auto handNetInputSize = op::flagsToPoint(op::String(FLAGS_hand_net_resolution), "368x368 (multiples of 16)");
        // poseMode
        const auto poseMode = op::flagsToPoseMode(FLAGS_body);
        // poseModel
        const auto poseModel = op::flagsToPoseModel(op::String(FLAGS_model_pose));
        // JSON saving
        if (!FLAGS_write_keypoint.empty())
            op::opLog(
                "Flag `write_keypoint` is deprecated and will eventually be removed. Please, use `write_json`"
                " instead.", op::Priority::Max);
        // keypointScaleMode
        const auto keypointScaleMode = op::flagsToScaleMode(FLAGS_keypoint_scale);
        // heatmaps to add
        const auto heatMapTypes = op::flagsToHeatMaps(FLAGS_heatmaps_add_parts, FLAGS_heatmaps_add_bkg,
                                                      FLAGS_heatmaps_add_PAFs);
        const auto heatMapScaleMode = op::flagsToHeatMapScaleMode(FLAGS_heatmaps_scale);
        // >1 camera view?
        const auto multipleView = (FLAGS_3d || FLAGS_3d_views > 1);
        // Face and hand detectors
        const auto faceDetector = op::flagsToDetector(FLAGS_face_detector);
        const auto handDetector = op::flagsToDetector(FLAGS_hand_detector);
        // Enabling Google Logging
        const bool enableGoogleLogging = true;

        // Pose configuration (use WrapperStructPose{} for default and recommended configuration)
        const op::WrapperStructPose wrapperStructPose{
            poseMode, netInputSize, outputSize, keypointScaleMode, FLAGS_num_gpu, FLAGS_num_gpu_start,
            FLAGS_scale_number, (float)FLAGS_scale_gap, op::flagsToRenderMode(FLAGS_render_pose, multipleView),
            poseModel, !FLAGS_disable_blending, (float)FLAGS_alpha_pose, (float)FLAGS_alpha_heatmap,
            FLAGS_part_to_show, op::String(FLAGS_model_folder), heatMapTypes, heatMapScaleMode, FLAGS_part_candidates,
            (float)FLAGS_render_threshold, FLAGS_number_people_max, FLAGS_maximize_positives, FLAGS_fps_max,
            op::String(FLAGS_prototxt_path), op::String(FLAGS_caffemodel_path),
            (float)FLAGS_upsampling_ratio, enableGoogleLogging};
        opWrapper.configure(wrapperStructPose);
        // Face configuration (use op::WrapperStructFace{} to disable it)
        const op::WrapperStructFace wrapperStructFace{
            FLAGS_face, faceDetector, faceNetInputSize,
            op::flagsToRenderMode(FLAGS_face_render, multipleView, FLAGS_render_pose),
            (float)FLAGS_face_alpha_pose, (float)FLAGS_face_alpha_heatmap, (float)FLAGS_face_render_threshold};
        opWrapper.configure(wrapperStructFace);
        // Hand configuration (use op::WrapperStructHand{} to disable it)
        const op::WrapperStructHand wrapperStructHand{
            FLAGS_hand, handDetector, handNetInputSize, FLAGS_hand_scale_number, (float)FLAGS_hand_scale_range,
            op::flagsToRenderMode(FLAGS_hand_render, multipleView, FLAGS_render_pose), (float)FLAGS_hand_alpha_pose,
            (float)FLAGS_hand_alpha_heatmap, (float)FLAGS_hand_render_threshold};
        opWrapper.configure(wrapperStructHand);
        // Extra functionality configuration (use op::WrapperStructExtra{} to disable it)
        const op::WrapperStructExtra wrapperStructExtra{
            FLAGS_3d, FLAGS_3d_min_views, FLAGS_identification, FLAGS_tracking, FLAGS_ik_threads};
        opWrapper.configure(wrapperStructExtra);
        // Output (comment or use default argument to disable any output)
        const op::WrapperStructOutput wrapperStructOutput{
            FLAGS_cli_verbose, op::String(FLAGS_write_keypoint), op::stringToDataFormat(FLAGS_write_keypoint_format),
            op::String(FLAGS_write_json), op::String(FLAGS_write_coco_json), FLAGS_write_coco_json_variants,
            FLAGS_write_coco_json_variant, op::String(FLAGS_write_images), op::String(FLAGS_write_images_format),
            op::String(FLAGS_write_video), FLAGS_write_video_fps, FLAGS_write_video_with_audio,
            op::String(FLAGS_write_heatmaps), op::String(FLAGS_write_heatmaps_format), op::String(FLAGS_write_video_3d),
            op::String(FLAGS_write_video_adam), op::String(FLAGS_write_bvh), op::String(FLAGS_udp_host),
            op::String(FLAGS_udp_port)};
        opWrapper.configure(wrapperStructOutput);
        // No GUI. Equivalent to: opWrapper.configure(op::WrapperStructGui{});
        // Set to single-thread (for sequential processing and/or debugging and/or reducing latency)
        if (FLAGS_disable_multi_thread)
            opWrapper.disableMultiThreading();
    }
    catch (const std::exception& e)
    {
        op::error(e.what(), __LINE__, __FUNCTION__, __FILE__);
    }
}

int frameNo = 0;
std::string outputFolder = "";
std::list<int> widthList;
void printImage(const std::shared_ptr<std::vector<std::shared_ptr<op::Datum>>>& datumProcessed) {
	widthList.push_back(getWidth(datumProcessed));
	std::cout << "print image, frame No: " << frameNo << std::endl;
	if (datumProcessed != nullptr)
	{
		try
		{
			if (datumProcessed != nullptr && !datumProcessed->empty())
			{
				// Write frame
				const cv::Mat cvMat = OP_OP2CVCONSTMAT(datumProcessed->at(0)->cvOutputData);
				std::string fileName = outputFolder+"/Frame - " + std::to_string(frameNo) + ".png";
				std::cout << fileName << std::endl;
				cv::Mat greyMat, thr;
				cv::cvtColor(cvMat, greyMat, cv::COLOR_BGR2GRAY);
				cv::threshold(greyMat, thr, 10, 255, cv::THRESH_BINARY);
				cv::imwrite(fileName, thr);
				frameNo++;
			}
			else
				op::opLog("Nullptr or empty datumsPtr found.", op::Priority::High);
		}
		catch (const std::exception& e)
		{
			op::error(e.what(), __LINE__, __FUNCTION__, __FILE__);
		}
	}
	else
		op::opLog("Frame could not be processed.", op::Priority::High);
}

int state = 0;
int s_count = 0;
bool doneProcessing = false;
int cycle_count = 1;
void processData(const std::shared_ptr<std::vector<std::shared_ptr<op::Datum>>>& datumsPtr) {
	std::cout << "**********************************************************************************************************" << std::endl;
	std::cout << "processing data, state: " << state << std::endl;
	s_count++;
	if (prevWidth == NULL) {
		prevWidth = getWidth(datumsPtr);
	}
	else {
		switch (state)
		{
		case 0:
			if (getDiff(datumsPtr) > 0) {
				state = 1;
			}
			break;
		case 1:
			if (getDiff(datumsPtr) < 0 && s_count > 0) {
				state = 2;
				s_count = 0;
			}
			break;
		case 2:
			if (getDiff(datumsPtr) > 0 && s_count > 0) {
				state = 3;
				s_count = 0;
			}
			if (cycle_count > 0) {
				printImage(datumsPtr);
			}
			break;
		case 3:
			if (getDiff(datumsPtr) < 0 && s_count > 0) {
				state = 4;
				s_count = 0;
			}
			if (cycle_count > 0) {
				printImage(datumsPtr);
			}
			break;
		case 4:
			if (getDiff(datumsPtr) > 0 && s_count > 0) {
				state = 5;
				s_count = 0;
			}
			if (cycle_count > 0) {
				printImage(datumsPtr);
			}
			break;
		case 5:
			if (getDiff(datumsPtr) < 0 && s_count > 0) {
				state = 6;
				s_count = 0;
			}
			if (cycle_count > 0) {
				printImage(datumsPtr);
			}
			break;
		case 6:
			cycle_count++;
			if (cycle_count < 2) {
				state = 1;
			}
			else {
				doneProcessing = true;
			}
			break;
		default:
			break;
		}

		prevWidth = getWidth(datumsPtr);
	}
}





void printCSV() {
	int count = 1;
	std::ofstream outfile;
	outfile.open("Widths.csv");
	std::list <int> ::iterator it;
	for (it = widthList.begin(); it != widthList.end(); ++it) {
		outfile << count++ << ',' << *it << std::endl;
	}
	outfile.close();
}

bool process_started = false;
std::string video_file_name;
void processVideo(op::Wrapper& opWrapper)
{
	cv::VideoCapture cap(video_file_name);
	std::cout << "start processing video" << std::endl;
	// Check if camera opened successfully
	if (!cap.isOpened()) {
		std::cout << "Error opening video stream or file" << std::endl;
	}

	//int frameNo = 0;
	while (1) {

		cv::Mat frame;
		//cv::Mat src;

		// Capture frame-by-frame
		cap >> frame;
		//cv::Size size(640,360);//the dst image size,e.g.100x100
		//resize(src, frame, size);//resize image
		// If the frame is empty, break immediately
		if (frame.empty() || doneProcessing)
			break;

		
		const op::Matrix frameToProcess = OP_CV2OPCONSTMAT(frame);
		auto datumProcessed = opWrapper.emplaceAndPop(frameToProcess);
		std::cout << "sending to process datum" << std::endl;
		//printImage(datumProcessed);
		//std::cout << validateDatum(datumProcessed) << std::endl;
		//widthList.push_back(getWidth(datumProcessed));

		if (!process_started) {
			process_started = validateDatum(datumProcessed);
		}
		if (process_started) {
			processData(datumProcessed);
			//widthList.push_back(getWidth(datumProcessed));
		}

			//if (datumProcessed != nullptr)
			//{
			//	try
			//	{
			//		if (datumProcessed != nullptr && !datumProcessed->empty())
			//		{
			//			// Write frame
			//			const cv::Mat cvMat = OP_OP2CVCONSTMAT(datumProcessed->at(0)->cvOutputData);
			//			std::string fileName = "C:/Users/Lasanga Madushan/Desktop/openpose/sample_out/Frame - "+ std::to_string(frameNo)+".png";
			//			std::cout << fileName << std::endl;
			//			cv::Mat greyMat, thr;
			//			cv::cvtColor(cvMat, greyMat, cv::COLOR_BGR2GRAY);
			//			cv::threshold(greyMat, thr, 10, 255, cv::THRESH_BINARY);
			//			cv::imwrite(fileName, thr);
			//		}
			//		else
			//			op::opLog("Nullptr or empty datumsPtr found.", op::Priority::High);
			//	}
			//	catch (const std::exception& e)
			//	{
			//		op::error(e.what(), __LINE__, __FUNCTION__, __FILE__);
			//	}
			//}
			//else
			//	op::opLog("Frame could not be processed.", op::Priority::High);
		
		
		//frameNo++;

	}

	// When everything done, release the video capture object
	cap.release();
	printCSV();
}

int tutorialApiCpp()
{
    try
    {
        op::opLog("Starting OpenPose demo...", op::Priority::High);
        const auto opTimer = op::getTimerInit();

        // Configuring OpenPose
        op::opLog("Configuring OpenPose...", op::Priority::High);
        op::Wrapper opWrapper{op::ThreadManagerMode::Asynchronous};
        configureWrapper(opWrapper);

        // Starting OpenPose
        op::opLog("Starting thread(s)...", op::Priority::High);
        opWrapper.start();

		processVideo(opWrapper);
		/*
        // Process and display image
        const cv::Mat cvImageToProcess = cv::imread(FLAGS_image_path);
        const op::Matrix imageToProcess = OP_CV2OPCONSTMAT(cvImageToProcess);
        auto datumProcessed = opWrapper.emplaceAndPop(imageToProcess);

        if (datumProcessed != nullptr)
        {
            printKeypoints(datumProcessed);
            if (!FLAGS_no_display)
                display(datumProcessed);
        }
        else
            op::opLog("Image could not be processed.", op::Priority::High);
			*/
        // Measuring total time
        op::printTime(opTimer, "OpenPose demo successfully finished. Total time: ", " seconds.", op::Priority::High);

        // Return
        return 0;
    }
    catch (const std::exception&)
    {
        return -1;
    }
}



int main(int argc, char *argv[])
{
    // Parsing command line flags
    gflags::ParseCommandLineFlags(&argc, &argv, true);
	video_file_name = argv[1];
	outputFolder = argv[2];
	std::cout << "input video: " << video_file_name << std::endl;
	std::cout << "output folder: " << outputFolder << std::endl;
	//boost::filesystem::remove_all("poses_over_gait_cycle");
	//boost::filesystem::create_directory("poses_over_gait_cycle");
	if (!CreateDirectory(outputFolder.c_str(), NULL)) {
		if (ERROR_ALREADY_EXISTS == GetLastError()) {
			std::cout << "folder already exist" << std::endl;
		}
		else {
			std::cout << "Problem with the output images path. Images are not saving...!" << std::endl;
		}
	}
    // Running tutorialApiCpp
    return tutorialApiCpp();
}

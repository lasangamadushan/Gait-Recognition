import os, shutil
import GEI_Builder
import recognizer


def recognize(input_video):
    #input_video = "videos/Asiri1.mp4"
    poses_folder = 'poses_over_gait_cycle'
    gei_folder = 'geis'
    gei_name = 'gei.png'

    def clean_folder(folder):
        for filename in os.listdir(folder):
            file_path = os.path.join(folder, filename)
            try:
                if os.path.isfile(file_path) or os.path.islink(file_path):
                    os.unlink(file_path)
                elif os.path.isdir(file_path):
                    shutil.rmtree(file_path)
            except Exception as e:
                print('Failed to delete %s. Reason: %s' % (file_path, e))


    # clear poses frames folder
    if(os.path.exists(poses_folder)):
        clean_folder(poses_folder)

    #generate pose images
    cmd_generate_pose_images = "03_keypoints_from_image.exe " + input_video +" "+ poses_folder
    os.system(cmd_generate_pose_images)


    #generate Gait Energy Image(GEI)
    if(not os.path.exists(gei_folder)):
        cmd_create_folder = "mkdir " + gei_folder
        os.system(cmd_create_folder)
    gei_out = gei_folder+'/'+gei_name
    GEI_Builder.build_gei(poses_folder, gei_out)
    return recognizer.recognize(gei_out)


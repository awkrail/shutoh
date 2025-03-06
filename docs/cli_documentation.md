# Shutoh CLI

## Quick start
**Shutoh** performs video shot detection. Inspired by [PySceneDetect](https://github.com/Breakthrough/PySceneDetect), it aims to provide a powerful and comparable alternative with enhanced performance. Shutoh supports the three main commands implemented in PySceneDetect: `list-scenes`, `split-video`, and `save-images`.

Save scene information as csv file:
```
shutoh -i input.mp4 -c list-scenes
```
Split input video on each fast cut and save them as videos:
```
shutoh -i input.mp4 -c split-video
```
Save frames from each cut:
```
shutoh -i input.mp4 -c save-images
```

## Options
Shutoh supports the above three commands with the five detectors. Detailed explainations about options are described below.
```
$shutoh --help
Usage: shutoh [--help] [--version] --input VAR --command VAR [--output VAR] [--filename VAR] [--no_output_file] [--copy] [--crf VAR] [--preset VAR] [--ffmpeg_args VAR] [--num_images VAR] [--format VAR] [--quality VAR] [--compression VAR] [--frame_margin VAR] [--scale VAR] [--width VAR] [--height VAR] [--start VAR] [--end VAR] [--duration VAR] [--detector VAR] [--threshold VAR] [--min_scene_len VAR] [--window_width VAR] [--min_content_val VAR] [--dct_size VAR] [--lowpass VAR] [--bins VAR] [--fade_bias VAR]
Optional arguments:
  -h, --help         shows help message and exits
  -v, --version      prints version information and exits
  -i, --input        Input video file. [required]
  -c, --command      Command name. choose one from [list-scenes, split-videos, save-images] [required]
  -o, --output       Output directory for created files. if unset, working directory will be used. [nargs=0..1] [default: "."]
  --filename         Output filename format to save csv, images, and videos. As with PySceneDetect, you can use macros like $VIDEO_NAME, $SCENE_NUMBER, $IMAGE_NUMBER. Default value: $VIDEO_NAME-scenes.csv (list-scenes), $VIDEO_NAME-scene-$SCENE_NUMBER (split-video), $VIDEO_NAME-scene-$SCENE_NUMBER-$IMAGE_NUMBER (save-images).
  --no_output_file   [list-scenes] Print scene list only.
  --copy             [split-video] Copy instead of re-encode. Faster but less precise.
  --crf              [split-video] Video encoding quality from 0 to 100, where lower is high quality. 0 is lossless. [nargs=0..1] [default: 22]
  --preset           [split-video] Video compression quality.Choose one from ultrafast, superfast, veryfast, faster, fast, medium, slow, slower, veryslow. [nargs=0..1] [default: "veryfast"]
  --ffmpeg_args      [split-video] Codec arguments passed to FFmpeg when splitting scenes.Use double quotes around arguments. Must specify at least audio/video codec. [nargs=0..1] [default: "-c:a aac -map 0:v:0 -map 0:a? -sn"]
  --num_images       [save-images] Number of images to generate per scene.Will always include start/end frame, unless -n=1,in which case the image will be the frame at the mid-point of the scene. [nargs=0..1] [default: 3]
  --format           [save-images] Output format of saved images. Choose one from jpg, png, and webp. [nargs=0..1] [default: "jpg"]
  --quality          [save-images] JPEG/WebP encoding quality, from 0 to 100. [nargs=0..1] [default: 95]
  --compression      [save-images] PNG compression rate, from 0-9.Higher values produce smaller files but result in longer compression time. [nargs=0..1] [default: 3]
  --frame_margin     [save-images] Number of frames to ignore at beginning/end of scenes when saving images.Controls temporal padding on scene boundaries. [nargs=0..1] [default: 1]
  --scale            [save-images] Factor to scale images. Ignored if -W/--width or -H/--height is set.
  -W, --width        [save-images] Width of images.
  -H, --height       [save-images] Height of images.
  --start            Time in video to start detection. Default value reperesents the first frame of the video.
  --end              Time in video to end detection. Default value represents the last frame of the video.
  --duration         Maximum time in video to process. Default value represents the whole video length. Ignored if --end is set.
  --detector         Detector type. Choose from [adaptive, content, hash, histogram, threshold]. [nargs=0..1] [default: "content"]
  --threshold        Threshold for scene shot detection. Higher values ignore small changes of scenes in the video.
  --min_scene_len    Minimum scene length (=#frames) in cuts. Higher values ignore abrupt cuts. [nargs=0..1] [default: 15]
  --window_width     [AdaptiveDetector]: Size of window (#frames) before/after to average together to detect deviations from the mean. [nargs=0..1] [default: 2]
  --min_content_val  [AdaptiveDetector]: Minimum threshold (float) that content_val must be over to register as a new scene. [nargs=0..1] [default: 15]
  --dct_size         [HashDetector]: Square size of low frequency to use for the DCT. [nargs=0..1] [default: 16]
  --lowpass          [HashDetector]: How much high frequency to filter from the DCT. A value of 2 means keeping lower 1/2 frequency data. [nargs=0..1] [default: 2]
  --bins             [HistogramDetector]: Number of bins to use for the histogram. [nargs=0..1] [default: 256]
  --fade_bias        [ThresholdDetector]: Float between -1.0 and +1.0 that represents the percentage of timecode skew for the start of a scene [nargs=0..1] [default: 0]
```

### General options
- `--help [-h]`: Show help messages
- `--version [-v]`: Show version information
- `--input [-i]`: Input video file path **(Required)**
- `--command [-c]`: Command name: `list-scenes`, `split-video`, and `save-images` **(Required)**
- `--output [-o]`: Output directory. If unset, working directory will be used.
- `--filename`: Output filename format to save csv, images, and videos. You can use macros like $VIDEO_NAME, $SCENE_NUMBER, anad $IMAGE_NUMBER. Default: $VIDEO_NAME-scenes.csv (`list-scenes`), $VIDEO_NAME-scene-$SCENE_NUMBER (`split-video`), and $VIDEO_NAME-scene-$SCENE_NUMBER-$IMAGE_NUMBER (`save-images`).

#### Examples
Save shots as csv with `scenes.csv` at `csv/` directory:
```
shutoh -i input.mp4 -c list-scenes -o csv --filename scenes.csv
```
Save frames from each shot as `frame_$SCENE_NUMBER_$IMAGE_NUMBER` at `frame/` directory:
```
shutoh -i input.mp4 -c save-images -o frame --filename frame_$SCENE_NUMBER_$IMAGE_NUMBER
```

### Command-specific Options
#### list-scenes
- `--no_output_file`: Print scenes only without saving csv files. In this mode, output-related options (e.g., `--output`) are ignored.
```
$shutoh -i video.mp4 -c list-scenes --no_output_file
scene_number,start_frame,start_time,end_frame,end_time
0,0,00:00:00.000,64,00:00:02.135
1,65,00:00:02.135,143,00:00:04.771
2,144,00:00:04.771,194,00:00:06.473
3,195,00:00:06.473,254,00:00:08.475
4,255,00:00:08.475,314,00:00:10.477
5,315,00:00:10.477,419,00:00:13.981
6,420,00:00:13.981,628,00:00:20.954
7,629,00:00:20.954,712,00:00:23.757
8,713,00:00:23.757,809,00:00:26.993
9,810,00:00:26.993,880,00:00:29.363
10,881,00:00:29.363,965,00:00:32.199
11,966,00:00:32.199,1125,00:00:37.537
12,1126,00:00:37.537,1211,00:00:40.407
13,1212,00:00:40.407,1250,00:00:41.708
14,1251,00:00:41.708,1330,00:00:44.377
15,1331,00:00:44.377,1391,00:00:46.412
...
```

#### split-video
- `--copy`: Copy instead of re-encoding (when saving, this will use `-copy` flag). Faster but less precise. [default: false]
- `--crf`: Constant Rate Factor. It is related to video encoding quality from 0 to 100, where lower is high quality. 0 is lossless. See [here](https://trac.ffmpeg.org/wiki/Encode/H.264) for details. [default: 22]
- `--preset`: Video compression quality. Choose one from ultrafast, superfast, veryfast, faster, fast, medium, slow, slower, veryslow. See [here](https://trac.ffmpeg.org/wiki/Encode/H.264#a2.Chooseapresetandtune) for details [default: veryfast]
- `--ffmpeg_args`: Codec arguments passed to FFmpeg when saving video shots. Use double quotes around arguments. Must specify at least audio/video codec. [default: "-c:a aac -map 0:v:0 -map 0:a? -sn"]

##### Examples
Change `--crf` into 30 and `--preset` into superfast:
```
shutoh -i input.mp4 -c split-video --crf 30 --preset superfast
```
Use `--copy`:
```
shutoh -i input.mp4 -c split-video --copy
```

#### save-images
- `--num_images`: Number of images to generate per scene.Will always include start/end frame, unless -n=1,in which case the image will be the frame at the mid-point of the scene. [default: 3]
- `--format`: Output format of saved images. Choose one from jpg, png, and webp. [default: jpg]
- `--quality`: JPEG/WebP encoding quality, from 0 to 100. 100 is lossless. [default: 95]
- `--compression`: PNG compression rate, from 0-9. Higher values produce smaller files but result in longer compression time. [default: 3]
- `--frame_margin`: Number of frames to ignore at beginning/end of scenes when saving images.Controls temporal padding on scene boundaries. [default: 1]
- `--width [-W]`: Width of images. If `-H/--height` is unset, height is computed by keeping the aspect ratio.
- `--height [-H]`: Height of images. If `-W/--width` is unset, width is computed by keeping the aspect ratio.
- `--scale`: Factor to scale images. Ignored if `-W/--width` or `-H/--height` is set.

##### Examples
Save 10 images from each cut:
```
shutoh -i input.mp4 -c save-images --num_images 10
```
Save images as webp format with quality = 85
```
shutoh -i input.mp4 -c save-images --format webp --quality 85
```
Resize images' width into 512
```
shutoh -i input.mp4 -c save-images --width 512
```
Rescale images into 3x
```
shutoh -i input.mp4 -c save-images --scale 3
```

### Detector-specific Options
Detailed explainations about detectors are described in the [PySceneDetect documentation](https://www.scenedetect.com/cli/).

Common parameters across the detectors are `--threshold` and `--min_scene_len`:
- `--threshold`: Threshold for scene shot detection. Higher values ignore small changes of scenes in the video (default value is different between the detectors).
- `--min_scene_len`: Minimum scene length (=#frames) in cuts. Higher values ignore abrupt cuts. [default: 15]

#### Adaptive detector
- `--window_width`: Size of sliding window (#frames) before/after to average together to detect deviations from the mean. [default: 2]
- `--min_content_val`: Minimum threshold (int) that content_val must be over to register as a new scene. [default: 15]

#### HashDetector
- `--dct_size`: Square size of low frequency to use for the DCT. [default: 16]
- `--lowpass`: How much high frequency to filter from the DCT. A value of 2 means keeping lower 1/2 frequency data. [default: 2]

#### HistogramDetector
- `--bins`: Number of bins to use for the histogram. [default: 256]

#### ThresholdDetector
- `--fade_bias`: Float between -1.0 and +1.0 that represents the percentage of timecode skew for the start of a scene [default: 0]

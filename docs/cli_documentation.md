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

**Example**
Save shots as csv with `scenes.csv` at `csv/` directory:
```
shutoh -i video.mp4 -c list-scenes -o csv --filename scenes.csv
```
Save frames from each shot as `frame_$SCENE_NUMBER_$IMAGE_NUMBER` at `frame/` directory:
```
shutoh -i video.mp4 -c save-images -o frame --filename frame_$SCENE_NUMBER_$IMAGE_NUMBER
```

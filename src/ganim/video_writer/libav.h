/** @file
 * @brief Includes the libav headers.
 *
 * These need to be in an `extern "C"` block, which can be annoying, so this
 * just includes them all with that `extern "C"` block for you.
 */
extern "C" {
#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

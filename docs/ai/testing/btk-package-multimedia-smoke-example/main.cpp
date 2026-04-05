#include <QtMultimedia/QAudioFormat>
#include <QtMultimedia/QMediaTimeRange>

int main()
{
   QAudioFormat format;
   format.setSampleRate(48000);
   format.setChannelCount(2);
   format.setSampleSize(16);

   QMediaTimeRange range;
   range.addInterval(10, 40);

   return ! format.isValid()
      || format.sampleRate() != 48000
      || format.channelCount() != 2
      || format.sampleSize() != 16
      || range.isEmpty()
      || range.earliestTime() != 10
      || range.latestTime() != 40 ? 1 : 0;
}

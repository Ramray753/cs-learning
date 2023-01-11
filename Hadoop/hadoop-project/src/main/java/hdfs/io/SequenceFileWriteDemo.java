package hdfs.io;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IOUtils;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.SequenceFile;
import org.apache.hadoop.io.Text;

import java.io.IOException;

public class SequenceFileWriteDemo {
  
  private static final String[] DATA = {
    "One, two, buckle my shoe",
    "Three, four, shut the door",
    "Five, six, pick up sticks",
    "Seven, eight, lay them straight",
    "Nine, ten, a big fat hen"
  };
  
  public static void main(String[] args) throws IOException {
    // Setup configurations
    String uri = args[0];
    Configuration conf = new Configuration();
    Path path = new Path(uri);
    IntWritable key = new IntWritable();
    Text value = new Text();

    // Transform to SequenceFile.Writer.Option
    SequenceFile.Writer.Option optPath = SequenceFile.Writer.file(path);
    SequenceFile.Writer.Option optKey = SequenceFile.Writer.keyClass(key.getClass());
    SequenceFile.Writer.Option optVal = SequenceFile.Writer.valueClass(value.getClass());
    // SequenceFile.Writer.Option optCom = SequenceFile.Writer.compression(SequenceFile.CompressionType.RECORD, new GzipCodec());

    // Write file
    SequenceFile.Writer writer = null;
    try {
      writer = SequenceFile.createWriter(conf, optPath, optKey, optVal);
      int numRow = 10000;
      for (int i = 0; i < numRow; i++) {
        key.set(numRow - i);
        value.set(DATA[i % DATA.length]);
        // Returns the current length of the output file.
        // writer.getLength()返回当前即将写入的的KV开始的位置，即上一对KV结束的位置
        System.out.printf("[%s]\t%s\t%s\n", writer.getLength(), key, value);
        writer.append(key, value);
      }
    } finally {
      IOUtils.closeStream(writer);
    }
  }
}

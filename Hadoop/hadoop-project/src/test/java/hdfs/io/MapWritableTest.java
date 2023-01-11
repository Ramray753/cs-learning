package hdfs.io;

import org.apache.hadoop.io.*;
import org.junit.Test;

import java.io.IOException;

import static org.hamcrest.CoreMatchers.is;
import static org.junit.Assert.assertThat;

public class MapWritableTest extends Serialize {
  
  @Test
  public void mapWritable() throws IOException {
    MapWritable src = new MapWritable();
    src.put(new IntWritable(1), new Text("cat"));
    src.put(new VIntWritable(2), new LongWritable(163));
    MapWritable dest = new MapWritable();
    /*
        Make a copy of the writable object using serialization to a buffer.
     */
    WritableUtils.cloneInto(dest, src);
    System.out.println("Dest kv1: " + dest.get(new IntWritable(1)));
    System.out.println("Dest kv2: " + dest.get(new VIntWritable(2)));
  }

  @Test
  public void setWritableEmulation() throws IOException {
    MapWritable src = new MapWritable();
    src.put(new IntWritable(1), NullWritable.get());
    src.put(new IntWritable(2), NullWritable.get());
    MapWritable dest = new MapWritable();
    WritableUtils.cloneInto(dest, src);
    assertThat(dest.containsKey(new IntWritable(1)), is(true));
  }
}

package hdfs.io;

import org.apache.hadoop.io.BytesWritable;
import org.apache.hadoop.util.StringUtils;
import org.junit.Test;

import java.io.IOException;

public class BytesWritableTest extends Serialize {
  
  @Test
  public void test() throws IOException {
    BytesWritable b = new BytesWritable(new byte[] { 3, 5 });
    byte[] bytes = serialize(b);
    System.out.println("Bytes hex: " + StringUtils.byteToHexString(bytes));
    b.setCapacity(11);
    System.out.println("Bytes length: " + b.getLength());
    System.out.println("Bytes capacity length:" + b.getBytes().length);
  }
}

package hdfs.io;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.*;
import org.junit.BeforeClass;
import org.junit.Test;

import java.io.IOException;

public class CheckSumTest {

    static FileSystem fs;

    @BeforeClass
    public static void setup() throws IOException {
        System.setProperty("HADOOP_USER_NAME", "hadoop");
        Configuration conf = new Configuration();
        conf.set("fs.defaultFS", "hdfs://hadoop-node-1:8020");
        fs = FileSystem.get(conf);
    }

    @Test
    public void testRead() {
        ChecksumFileSystem cfs = new ChecksumFileSystem(fs){};
        Path checksumFile = cfs.getChecksumFile(new Path("hdfs://hadoop-node-1/mr_data/input/ncdc/sample.txt"));
        System.out.println(checksumFile);
    }
}

package hdfs.quickstart;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FSDataInputStream;
import org.apache.hadoop.fs.FSDataOutputStream;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IOUtils;
import org.junit.BeforeClass;
import org.junit.Test;

import java.io.*;

public class FileSystemTest {

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
        InputStream in = null;
        try {
            in = fs.open(new Path("hdfs://hadoop-node-1/io_test/text"));
            IOUtils.copyBytes(in, System.out, 4096, false);
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            IOUtils.closeStream(in);
        }
    }

    @Test
    public void testReadTwice() {
        FSDataInputStream in = null;
        try {
            in = fs.open(new Path("hdfs://hadoop-node-1/io_test/text"));
            IOUtils.copyBytes(in, System.out, 4096, false);
            in.seek(0);
            IOUtils.copyBytes(in, System.out, 4096, false);
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            IOUtils.closeStream(in);
        }
    }

    @Test
    public void testWrite() throws IOException {
        InputStream in = new BufferedInputStream(new FileInputStream("src/main/resources/new.txt"));
        FSDataOutputStream out = fs.create(new Path("hdfs://hadoop-node-1/io_test/new_remote.txt"),
                () -> System.out.print("."));
        IOUtils.copyBytes(in, out, 4096, true);
    }
}

package hdfs.quickstart;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileStatus;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.FileUtil;
import org.apache.hadoop.fs.Path;
import org.junit.After;
import org.junit.BeforeClass;
import org.junit.Test;

import java.io.IOException;

public class ListStatus {

    private static FileSystem fs;

    @BeforeClass
    public static void setup() throws IOException {
        Configuration conf = new Configuration();
        System.setProperty("HADOOP_USER_NAME", "hadoop");
        conf.set("fs.defaultFS", "hdfs://hadoop-node-1:8020");
        fs = FileSystem.get(conf);
    }

    @After
    public void tearDown() throws IOException {
        if (fs != null) { fs.close(); }
    }

    @Test
    public void testListStatus() throws IOException {
        Path[] paths = new Path[]{new Path("hdfs://hadoop-node-1:8020/")};
        FileStatus[] status = fs.listStatus(paths);
        Path[] listedPaths = FileUtil.stat2Paths(status);
        for (Path p : listedPaths) {
            System.out.println(p);
        }
    }
}

package hdfs.quickstart;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileStatus;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import java.io.FileNotFoundException;
import java.io.IOException;

public class ShowFileStatusTest {

    private FileSystem fs;

    @Before
    public void setUp() throws IOException {
        Configuration conf = new Configuration();
        System.setProperty("HADOOP_USER_NAME", "hadoop");
        conf.set("fs.defaultFS", "hdfs://hadoop-node-1:8020");
        fs = FileSystem.get(conf);
    }

    @After
    public void tearDown() throws IOException {
        if (fs != null) { fs.close(); }
    }

    @Test(expected = FileNotFoundException.class)
    public void throwsFileNotFoundForNonExistentFile() throws IOException {
        fs.getFileStatus(new Path("no-such-file"));
    }

    @Test
    public void fileStatusForFile() throws IOException {
        FileStatus stat = fs.getFileStatus(new Path("hdfs://hadoop-node-1:8020/io_test/new_remote.txt"));
        System.out.println("FileStatus for file:");
        showFileStatus(stat);
    }

    @Test
    public void fileStatusForDirectory() throws IOException {
        FileStatus stat = fs.getFileStatus(new Path("hdfs://hadoop-node-1:8020/io_test"));
        System.out.println("FileStatus for directory:");
        showFileStatus(stat);
    }

    public static void showFileStatus(FileStatus stat){
        System.out.println("Path: " + stat.getPath().toUri().getPath());
        System.out.println("Is directory: " + stat.isDirectory());
        System.out.println("Length: " + stat.getLen());
        System.out.println("Modification time: " + stat.getModificationTime());
        System.out.println("Replica: " + stat.getReplication());
        System.out.println("Block size: " + stat.getBlockSize());
        System.out.println("Owner: " + stat.getOwner());
        System.out.println("Group: " + stat.getGroup());
        System.out.println("permission: " + stat.getPermission());
    }

}
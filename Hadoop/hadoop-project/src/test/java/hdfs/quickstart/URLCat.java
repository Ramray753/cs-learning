package hdfs.quickstart;

import org.apache.hadoop.fs.FsUrlStreamHandlerFactory;
import org.apache.hadoop.io.IOUtils;
import org.junit.BeforeClass;
import org.junit.Test;

import java.io.IOException;
import java.io.InputStream;
import java.net.MalformedURLException;
import java.net.URL;

public class URLCat {
    @BeforeClass
    public static void setup(){
        URL.setURLStreamHandlerFactory(new FsUrlStreamHandlerFactory());
    }

    @Test
    public void testURL() throws MalformedURLException {
        InputStream in = null;
        try {
            in = new URL("hdfs://hadoop-node-1/io_test/new_remote.txt").openStream();
            IOUtils.copyBytes(in, System.out, 4096, false);
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            IOUtils.closeStream(in);
        }
    }
}

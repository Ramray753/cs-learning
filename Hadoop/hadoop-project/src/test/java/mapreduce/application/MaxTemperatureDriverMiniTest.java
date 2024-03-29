package mapreduce.application;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileUtil;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.fs.PathFilter;
import org.apache.hadoop.mapred.ClusterMapReduceTestCase;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;

import static org.hamcrest.CoreMatchers.is;
import static org.hamcrest.CoreMatchers.nullValue;
import static org.junit.Assert.assertThat;

public class MaxTemperatureDriverMiniTest extends ClusterMapReduceTestCase {

    @Override
    public void setUp() throws Exception {
        if (System.getProperty("test.build.data") == null) {
            System.setProperty("test.build.data", "/tmp");
        }
        if (System.getProperty("hadoop.log.dir") == null) {
            System.setProperty("hadoop.log.dir", "/tmp");
        }
        super.setUp();
    }

    // Not marked with @Test since ClusterMapReduceTestCase is a JUnit 3 test case
    public void test() throws Exception {
        Configuration conf = createJobConf();

        Path localInput = new Path("input/cp6/micro");
        Path input = getInputDir(); // 默认：target/input
        Path output = getOutputDir(); // 默认：target/output

        // Copy input data into test HDFS
        getFileSystem().copyFromLocalFile(localInput, input);

        MaxTemperatureDriver driver = new MaxTemperatureDriver();
        driver.setConf(conf);

        int exitCode = driver.run(new String[]{input.toString(), output.toString()});
        assertThat(exitCode, is(0));

        // Check the output is as expected
        Path[] outputFiles = FileUtil.stat2Paths(getFileSystem().listStatus(output, new PathFilter() {
            @Override
            public boolean accept(Path path) {
                return !path.getName().startsWith("_");
            }
        }));
        assertThat(outputFiles.length, is(1));

        InputStream in = getFileSystem().open(outputFiles[0]);
        BufferedReader reader = new BufferedReader(new InputStreamReader(in));
        assertThat(reader.readLine(), is("1949\t111"));
        assertThat(reader.readLine(), is("1950\t22"));
        assertThat(reader.readLine(), nullValue());
        reader.close();
    }
}

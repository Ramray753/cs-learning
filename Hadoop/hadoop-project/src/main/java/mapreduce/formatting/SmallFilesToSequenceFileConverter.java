package mapreduce.formatting;

import org.apache.hadoop.conf.Configured;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.BytesWritable;
import org.apache.hadoop.io.NullWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.InputSplit;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.lib.input.FileSplit;
import org.apache.hadoop.mapreduce.lib.output.SequenceFileOutputFormat;
import org.apache.hadoop.util.Tool;
import org.apache.hadoop.util.ToolRunner;

import java.io.IOException;

public class SmallFilesToSequenceFileConverter extends Configured implements Tool {

    static class SequenceFileMapper extends Mapper<NullWritable, BytesWritable, Text, BytesWritable> {
        private Text filenameKey;

        @Override
        protected void setup(Context context) throws IOException, InterruptedException {
            // 设置Key
            InputSplit split = context.getInputSplit();
            Path path = ((FileSplit) split).getPath();
            filenameKey = new Text(path.toString());
        }

        @Override
        protected void map(NullWritable key, BytesWritable value, Context context) throws IOException, InterruptedException {
            // 直接输出：一个Map处理一个分片，一个分片就是一个文件
            context.write(filenameKey, value);
        }

    }

    @Override
    public int run(String[] args) throws Exception {
        Job job = JobBuilder.parseInputAndOutput(this, getConf(), args);
        if (job == null) {
            return -1;
        }
        // 设置输入输出格式
        job.setInputFormatClass(WholeFileInputFormat.class);
        job.setOutputFormatClass(SequenceFileOutputFormat.class);

        // 设置输出类型，输入类型由输入格式确定：NullWritable和BytesWritable
        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(BytesWritable.class);

        // 设置Mapper类型
        job.setMapperClass(SequenceFileMapper.class);

        return job.waitForCompletion(true) ? 0 : 1;
    }

    public static void main(String[] args) throws Exception {
        int exitCode = ToolRunner.run(new SmallFilesToSequenceFileConverter(), args);
        System.exit(exitCode);
    }

    public static final int NOT_A_VERTEX = -1;

    /**
     * Compute all-shortest paths.
     * a[ ][ ] contains the adjacency matrix with
     * a[ i ][ i ] presumed to be zero.
     * d[ ] contains the values of the shortest path.
     * Vertices are numbered starting at 0; all arrays
     * have equal dimension. A negative cycle exists if
     * d[ i ][ i ] is set to a negative value.
     * Actual path can be computed using path[ ][ ].
     * NOT_A_VERTEX is -1
     */
    public static void allPairs(int[][] a, int[][] d, int[][] path) {
        int n = a.length;

        // Initialize d and path
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) {
                d[i][j] = a[i][j];
                path[i][j] = NOT_A_VERTEX;
            }

        for (int k = 0; k < n; k++)
            // Consider each vertex as an intermediate
            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++)
                    if (d[i][k] + d[k][j] < d[i][j]) {
                        // Update shortest path
                        d[i][j] = d[i][k] + d[k][j];
                        path[i][j] = k;
                    }
    }

}
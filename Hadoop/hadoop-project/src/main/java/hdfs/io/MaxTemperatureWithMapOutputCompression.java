package hdfs.io;

import mapreduce.quickstart.MaxTempMapper;
import mapreduce.quickstart.MaxTempReducer;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.compress.CompressionCodec;
import org.apache.hadoop.io.compress.GzipCodec;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

public class MaxTemperatureWithMapOutputCompression {

    public static void main(String[] args) throws Exception {
        if (args.length != 2) {
            System.err.println("Usage: MaxTemperatureWithCompression <input path> " + "<output path>");
            System.exit(-1);
        }
        // 配置文件对象
        Configuration conf = new Configuration();
        conf.setBoolean(Job.MAP_OUTPUT_COMPRESS, true);
        conf.setClass(Job.MAP_OUTPUT_COMPRESS_CODEC, GzipCodec.class, CompressionCodec.class);
        // 创建作业实例，参数：配置对象，名字
        Job job = Job.getInstance(conf, MaxTemperatureWithMapOutputCompression.class.getSimpleName());

        /* 设置作业 */
        // 设置作业驱动
        job.setJobName(MaxTemperatureWithMapOutputCompression.class.getSimpleName());
        job.setJarByClass(MaxTemperatureWithMapOutputCompression.class);
        // 设置作业Map阶段输出KV类型
        job.setMapOutputKeyClass(Text.class);
        job.setMapOutputValueClass(IntWritable.class);
        // 设置作业Reduce阶段输出KV类型，也是程序的最终输出类型
        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(IntWritable.class);
        // 设置作业Mapper和Reducer类
        job.setMapperClass(MaxTempMapper.class);
        job.setCombinerClass(MaxTempReducer.class);
        job.setReducerClass(MaxTempReducer.class);

        /* 设置输入输出文件格式 */
        // 配置作业输入数据的路径，注意不使用过片Job，而是通过默认组件设置
        FileInputFormat.addInputPath(job, new Path(args[0]));
        // 配置作业输出数据的路径
        FileOutputFormat.setOutputPath(job, new Path(args[1]));
        // 设置输出压缩
        FileOutputFormat.setCompressOutput(job, true);
        FileOutputFormat.setOutputCompressorClass(job, GzipCodec.class);

        /* 判断输出路径是否存在，若存在则删除 */
        FileSystem fs = FileSystem.get(conf);
        if (fs.exists(new Path(args[1]))){
          fs.delete(new Path(args[1]), true);
        }

        /* 提交作业并等待作业完成 */
        System.exit(job.waitForCompletion(true) ? 0 : 1);
    }
}

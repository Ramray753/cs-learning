package mapreduce.quickstart;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.Writable;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

import java.io.IOException;

public class MRDriverFactory {
    public static void init(String[] args, boolean setCombiner, Class<?> driver,
                      Class<? extends Mapper<?,?,?,?>> mapper,
                      Class<? extends Reducer<?,?,?,?>> reducer,
                      Class<? extends Writable> K1,
                      Class<? extends Writable> V1,
                      Class<? extends Writable> K2,
                      Class<? extends Writable> V2) throws IOException, InterruptedException, ClassNotFoundException {
        if (args.length != 2) {
            System.err.println("Usage: hadoop jar <file> <input path> <output path>");
            System.exit(-1);
        }
        // 配置文件对象
        Configuration conf = new Configuration();
        // 创建作业实例，参数：配置对象，名字
        Job job = Job.getInstance(conf, driver.getSimpleName());
        // 设置作业驱动
        job.setJarByClass(driver);
        job.setJobName(driver.getSimpleName());
        // 配置作业输入数据的路径
        FileInputFormat.addInputPath(job, new Path(args[0]));
        // 配置作业输出数据的路径
        FileOutputFormat.setOutputPath(job, new Path(args[1]));
        // 设置作业Mapper和Reducer类
        job.setMapperClass(mapper);
        job.setReducerClass(reducer);
        if (setCombiner)
            job.setCombinerClass(reducer);
        // 设置作业Map阶段输出KV类型
        job.setMapOutputKeyClass(K1);
        job.setMapOutputValueClass(V1);
        // 设置作业Reduce阶段输出KV类型，也是程序的最终输出类型
        job.setOutputKeyClass(K2);
        job.setOutputValueClass(V2);
        // 判断输出路径是否存在，若存在则删除
        FileSystem fs = FileSystem.get(conf);
        if (fs.exists(new Path(args[1]))){
            fs.delete(new Path(args[1]), true);
        }
        // 提交作业并等待作业完成
        System.exit(job.waitForCompletion(true) ? 0 : 1);
    }
}

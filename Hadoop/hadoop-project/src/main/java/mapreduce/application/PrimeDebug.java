package mapreduce.application;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.NullWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

import java.io.IOException;

public class PrimeDebug extends Mapper<LongWritable, Text, IntWritable, NullWritable> {

    @Override
    public void map(LongWritable key, Text value, Context context)
            throws IOException, InterruptedException {
        int i = Integer.parseInt(value.toString());
        if (i <= 0){
            System.err.println("Negative or zero input detected: " + value);
            context.setStatus("Detected possibly corrupt record: see logs.");
            return;
        }
        if (isPrime(i)){
            context.write(new IntWritable(i), NullWritable.get());
            context.getCounter("MyCounter", "PRIME").increment(1);
        }
    }

    private static boolean isPrime(int n) {
        if (n == 2 || n == 3)
            return true;
        if (n == 1 || n % 2 == 0)
            return false;
        for (int i = 3; i * i <= n; i += 2)
            if (n % i == 0)
                return false;
        return true;
    }

    public static void main(String[] args) throws IOException, InterruptedException, ClassNotFoundException {
        if (args.length != 2) {
            System.err.println("Usage: hadoop jar <file> <input path> <output path>");
            System.exit(-1);
        }
        // 配置文件对象
        Configuration conf = new Configuration();
        // 创建作业实例，参数：配置对象，名字
        Job job = Job.getInstance(conf, PrimeDebug.class.getSimpleName());
        // 设置作业驱动
        job.setJarByClass(PrimeDebug.class);
        job.setJobName(PrimeDebug.class.getSimpleName());
        // 配置作业输入数据的路径
        FileInputFormat.addInputPath(job, new Path(args[0]));
        // 配置作业输出数据的路径
        FileOutputFormat.setOutputPath(job, new Path(args[1]));
        // 设置作业Mapper和Reducer类
        job.setMapperClass(PrimeDebug.class);
        job.setNumReduceTasks(0);
        // 设置作业Map阶段输出KV类型
        job.setMapOutputKeyClass(IntWritable.class);
        job.setMapOutputValueClass(NullWritable.class);
        // 判断输出路径是否存在，若存在则删除
        FileSystem fs = FileSystem.get(conf);
        if (fs.exists(new Path(args[1]))){
            fs.delete(new Path(args[1]), true);
        }
        // 提交作业并等待作业完成
        System.exit(job.waitForCompletion(true) ? 0 : 1);
    }
}


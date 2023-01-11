package mapreduce.feature;

import mapreduce.formatting.JobBuilder;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.conf.Configured;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.SequenceFile.CompressionType;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.compress.DeflateCodec;
import org.apache.hadoop.io.compress.GzipCodec;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.SequenceFileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.SequenceFileOutputFormat;
import org.apache.hadoop.mapreduce.lib.partition.InputSampler;
import org.apache.hadoop.mapreduce.lib.partition.TotalOrderPartitioner;
import org.apache.hadoop.util.Tool;
import org.apache.hadoop.util.ToolRunner;

import java.net.URI;

public class SortByTemperatureUsingTotalOrderPartitioner extends Configured implements Tool {

    @Override
    public int run(String[] args) throws Exception {
        Job job = JobBuilder.parseInputAndOutput(this, getConf(), args);
        if (job == null) {
            return -1;
        }
        // 设置输入输出格式与类型
        job.setInputFormatClass(SequenceFileInputFormat.class);
        job.setOutputKeyClass(IntWritable.class);
        job.setOutputFormatClass(SequenceFileOutputFormat.class);
        SequenceFileOutputFormat.setCompressOutput(job, true);
        SequenceFileOutputFormat.setOutputCompressorClass(job, DeflateCodec.class);
        SequenceFileOutputFormat.setOutputCompressionType(job, CompressionType.BLOCK);

        // 设定分区器
        job.setPartitionerClass(TotalOrderPartitioner.class);

        // 新建一个Sampler，泛型为Map的输出类型
        // RandomSampler是Sampler接口的一个实现
        // Sample from random points in the input. General-purpose sampler.
        // Takes numSamples / maxSplitsSampled inputs from each split.
        // 构造函数参数：
        //              freq – Probability with which a key will be chosen.
        //              numSamples – Total number of samples to obtain from all selected splits.
        //              maxSplitsSampled – The maximum number of splits to examine.
        InputSampler.Sampler<IntWritable, Text> sampler = new InputSampler.RandomSampler<>(0.1, 10000, 10);

        // Write a partition file for the given job, using the Sampler provided.
        // Queries the sampler for a sample keyset, sorts by the output key comparator,
        // selects the keys for each rank, and writes to the destination returned from
        // TotalOrderPartitioner.getPartitionFile.
        InputSampler.writePartitionFile(job, sampler);

        // Add to DistributedCache
        Configuration conf = job.getConfiguration();
        String partitionFile = TotalOrderPartitioner.getPartitionFile(conf);
        URI partitionUri = new URI(partitionFile);
        job.addCacheFile(partitionUri);

        return job.waitForCompletion(true) ? 0 : 1;
    }

    public static void main(String[] args) throws Exception {
        int exitCode = ToolRunner.run(new SortByTemperatureUsingTotalOrderPartitioner(), args);
        System.exit(exitCode);
    }
}
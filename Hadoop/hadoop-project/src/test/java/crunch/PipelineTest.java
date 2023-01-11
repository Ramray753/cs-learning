package crunch;

import com.google.common.base.Charsets;
import com.google.common.io.Files;
import org.apache.crunch.*;
import org.apache.crunch.impl.mr.MRPipeline;
import org.apache.crunch.io.At;
import org.apache.crunch.io.To;
import org.apache.crunch.test.TemporaryPath;
import org.apache.crunch.types.writable.Writables;
import org.junit.Rule;
import org.junit.Test;

import java.io.File;
import java.io.IOException;
import java.io.Serializable;

public class PipelineTest implements Serializable {

    @Rule
    public transient TemporaryPath tmpDir = new TemporaryPath();

    @Test
    public void test1() throws IOException, InterruptedException {
        Pipeline pipeline = new MRPipeline(getClass());
        String inputPath = tmpDir.copyResourceFileName("crunch/set2.txt");
        // 读取数据
        PCollection<String> lines = pipeline.readTextFile(inputPath);
        // 转化为小写字母
        DoFn<String, String> doFn1 = new DoFn<String, String>() {
            @Override
            public void process(String input, Emitter<String> emitter) {
                emitter.emit(input.toLowerCase());
            }
        };
        PCollection<String> lower = lines.parallelDo("lower", doFn1, Writables.strings());
        // 统计所有单词出现的次数
        PTable<String, Long> counts = lower.count();
        // 交换KV
        DoFn<Pair<String, Long>, Pair<Long, String>> doFn2 = new DoFn<Pair<String, Long>, Pair<Long, String>>() {
            @Override
            public void process(Pair<String, Long> input, Emitter<Pair<Long, String>> emitter) {
                emitter.emit(Pair.of(input.second(), input.first()));
            }
        };
        PTable<Long, String> inverseCounts = counts.parallelDo("inverse", doFn2, Writables.tableOf(Writables.longs(), Writables.strings()));
        // 写入检查点
        inverseCounts.write(At.sequenceFile("output/checkpoint", inverseCounts.getPType()), Target.WriteMode.CHECKPOINT);
        // 计算相同出现的次数的单词的个数
        MapFn<Iterable<String>, Integer> mapFn = new MapFn<Iterable<String>, Integer>() {
            @Override
            public Integer map(Iterable<String> input) {
                int count = 0;
                for (String ignored : input) {
                    count++;
                }
                return count;
            }
        };
        PTable<Long, Integer> hist = inverseCounts
                .groupByKey()
                .mapValues("count values", mapFn, Writables.ints());
        // 写入文件
        hist.write(To.textFile("output/count"), Target.WriteMode.OVERWRITE);
        // 执行管线
        PipelineExecution execution = pipeline.runAsync();
        // 写入DOT文件
        String dot = execution.getPlanDotFile();
        Files.write(dot, new File("output/pipeline.dot"), Charsets.UTF_8);
        execution.waitUntilDone();
        // 执行结束后清理管线
        pipeline.done();
    }

}

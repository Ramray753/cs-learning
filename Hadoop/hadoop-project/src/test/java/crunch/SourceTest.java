package crunch;

import org.apache.crunch.*;
import org.apache.crunch.impl.mr.MRPipeline;
import org.apache.crunch.test.TemporaryPath;
import org.apache.crunch.types.writable.Writables;
import org.junit.Rule;
import org.junit.Test;

import java.io.IOException;
import java.io.Serializable;

public class SourceTest implements Serializable {

    @Rule
    public transient TemporaryPath tmpDir = new TemporaryPath();

    @Test
    public void test1() throws IOException {
        Pipeline pipeline = new MRPipeline(getClass());
        String inputPath = tmpDir.copyResourceFileName("crunch/sample.txt");
        PCollection<String> lines = pipeline.readTextFile(inputPath);
        DoFn<String, String> doFn = new DoFn<String, String>() {
            @Override
            public void process(String input, Emitter<String> emitter) {
                emitter.emit(input.toLowerCase());
            }
        };
        PCollection<String> lower = lines.parallelDo(doFn, Writables.strings());
        Iterable<String> materialized = lower.materialize();
        for (String s : materialized) { // pipeline is run
            System.out.println(s);
        }
        pipeline.run();
    }


}

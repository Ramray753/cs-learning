package crunch;

import applications.crunch.NcdcRecordParser;
import org.apache.crunch.DoFn;
import org.apache.crunch.Emitter;
import org.apache.crunch.PCollection;
import org.apache.crunch.Pipeline;
import org.apache.crunch.impl.mr.MRPipeline;
import org.apache.crunch.io.From;
import org.apache.crunch.lib.Sort;
import org.apache.crunch.test.TemporaryPath;
import org.apache.crunch.types.avro.Avros;
import org.apache.crunch.types.writable.WritableTypeFamily;
import org.junit.Rule;
import org.junit.Test;

import java.io.IOException;
import java.io.Serializable;

import static crunch.Utils.dump;

public class PTypeTest implements Serializable {

    @Rule
    public transient TemporaryPath tmpDir = new TemporaryPath();

    @Test
    public void test1() throws IOException {
        String inputPath = tmpDir.copyResourceFileName("crunch/sample.txt");
        Pipeline pipeline = new MRPipeline(getClass());
        PCollection<String> lines = pipeline.read(From.textFile(inputPath));
        /*
          PType.getFamily(): Returns the PTypeFamily that this PType belongs to.
         */
        System.out.println(lines.getPType());
        System.out.println(lines.getPType().getFamily());
        System.out.println(WritableTypeFamily.getInstance());
    }

    @Test
    public void test2() throws IOException {
        String inputPath = tmpDir.copyResourceFileName("crunch/sample.txt");
        Pipeline pipeline = new MRPipeline(getClass());
        PCollection<String> lines = pipeline.read(From.textFile(inputPath, Avros.strings()));
        System.out.println(lines.getPType());
        System.out.println(lines.getPType().getFamily());
        System.out.println(Utils.dump(lines));
    }

    @Test
    public void test3() throws IOException {
        String inputPath = tmpDir.copyResourceFileName("crunch/sample.txt");
        Pipeline pipeline = new MRPipeline(getClass());
        PCollection<String> lines = pipeline.read(From.textFile(inputPath));
        DoFn<String, WeatherRecord> doFn = new DoFn<String, WeatherRecord>() {
            final NcdcRecordParser parser = new NcdcRecordParser();
            @Override
            public void process(String input, Emitter<WeatherRecord> emitter) {
                parser.parse(input);
                if (parser.isValidTemperature()) {
                    emitter.emit(new WeatherRecord(parser.getYearInt(), parser.getAirTemperature(), parser.getStationId()));
                }
            }
        };
        PCollection<WeatherRecord> records = lines.parallelDo(doFn, Avros.records(WeatherRecord.class));
        PCollection<WeatherRecord> sortedRecords = Sort.sort(records);
        System.out.println(records.getPType());
        System.out.println(records.getPType().getFamily());
        System.out.println(Utils.dump(sortedRecords));
    }

}

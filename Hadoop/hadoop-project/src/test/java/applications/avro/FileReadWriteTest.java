package applications.avro;

import org.apache.avro.Schema;
import org.apache.avro.file.DataFileReader;
import org.apache.avro.file.DataFileWriter;
import org.apache.avro.generic.GenericData;
import org.apache.avro.generic.GenericDatumReader;
import org.apache.avro.generic.GenericDatumWriter;
import org.apache.avro.generic.GenericRecord;
import org.apache.avro.io.DatumReader;
import org.apache.avro.io.DatumWriter;
import org.junit.Test;

import java.io.File;
import java.io.IOException;

public class FileReadWriteTest {
    @Test
    public void readTest()throws IOException {
        // 解析Avro文件
        File file = new File("output/data.avro");
        DatumReader<GenericRecord> reader = new GenericDatumReader<>();
        DataFileReader<GenericRecord> dataFileReader = new DataFileReader<>(file, reader);
        // 读取Schema
        Schema.Parser parser = new Schema.Parser();
        Schema schema = parser.parse(StringPairTest.class.getResourceAsStream("/avro/StringPair.avsc"));
        System.out.println(schema.equals(dataFileReader.getSchema()));
        // 读取数据
        System.out.println(dataFileReader.hasNext());
        GenericRecord result = null;
        while(dataFileReader.hasNext()){
            result = dataFileReader.next(result);
            System.out.println("left: " + result.get("left"));
            System.out.println("right: " + result.get("right"));
        }
    }

    @Test
    public void writeTest() throws IOException {
        // 解析模式
        Schema.Parser parser = new Schema.Parser();
        Schema schema = parser.parse(StringPairTest.class.getResourceAsStream("/avro/StringPair.avsc"));
        GenericRecord datum = new GenericData.Record(schema);
        datum.put("left", "L");
        datum.put("right", "R");
        // 写入文件
        File file = new File("output/data.avro");
        DatumWriter<GenericRecord> writer = new GenericDatumWriter<>(schema);
        DataFileWriter<GenericRecord> dataFileWriter = new DataFileWriter<>(writer);
        dataFileWriter.create(schema, file);
        dataFileWriter.append(datum);
        dataFileWriter.close();
    }
}

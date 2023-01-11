package applications.parquet;

import org.apache.avro.Schema;
import org.apache.avro.generic.GenericData;
import org.apache.avro.generic.GenericRecord;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.parquet.avro.AvroParquetReader;
import org.apache.parquet.avro.AvroParquetWriter;
import org.apache.parquet.avro.AvroReadSupport;
import org.apache.parquet.column.ParquetProperties;
import org.apache.parquet.example.data.Group;
import org.apache.parquet.example.data.GroupFactory;
import org.apache.parquet.example.data.simple.SimpleGroupFactory;
import org.apache.parquet.hadoop.ParquetReader;
import org.apache.parquet.hadoop.ParquetWriter;
import org.apache.parquet.hadoop.example.GroupReadSupport;
import org.apache.parquet.hadoop.example.GroupWriteSupport;
import org.apache.parquet.schema.MessageType;
import org.apache.parquet.schema.MessageTypeParser;
import org.junit.Test;

import java.io.IOException;

import static org.hamcrest.CoreMatchers.is;
import static org.junit.Assert.*;

public class ParquetTest {

    @Test
    public void testParquetFile() throws IOException {
        MessageType schema = MessageTypeParser.parseMessageType(
                "message Pair {\n" +
                        "  required binary left (UTF8);\n" +
                        "  required binary right (UTF8);\n" +
                        "}");

        GroupFactory groupFactory = new SimpleGroupFactory(schema);
        Group group = groupFactory.newGroup()
                .append("left", "L")
                .append("right", "R");

        Configuration conf = new Configuration();
        Path path = new Path("./output/parquet/data.parquet");
        FileSystem fileSystem = FileSystem.get(conf);
        if (fileSystem.exists(path.getParent()))
            fileSystem.delete(path.getParent(),true);
        GroupWriteSupport writeSupport = new GroupWriteSupport();
        GroupWriteSupport.setSchema(schema, conf);
        ParquetWriter<Group> writer = new ParquetWriter<>(path, writeSupport,
                ParquetWriter.DEFAULT_COMPRESSION_CODEC_NAME,
                ParquetWriter.DEFAULT_BLOCK_SIZE,
                ParquetWriter.DEFAULT_PAGE_SIZE,
                ParquetWriter.DEFAULT_PAGE_SIZE, /* dictionary page size */
                ParquetWriter.DEFAULT_IS_DICTIONARY_ENABLED,
                ParquetWriter.DEFAULT_IS_VALIDATING_ENABLED,
                ParquetProperties.WriterVersion.PARQUET_1_0, conf);
        writer.write(group);
        writer.close();

        GroupReadSupport readSupport = new GroupReadSupport();
        ParquetReader<Group> reader = new ParquetReader<>(path, readSupport);
        Group result;
        while ((result = reader.read()) != null){
            System.out.println(result.getString("left", 0));
            System.out.println(result.getString("right", 0));
        }
    }

    @Test
    public void testParquetFileUsingAvro() throws IOException {
        Schema.Parser parser = new Schema.Parser();
        Schema schema = parser.parse(getClass().getResourceAsStream("/avro/StringPair.avsc"));

        GenericRecord datum = new GenericData.Record(schema);
        datum.put("left", "L");
        datum.put("right", "R");

        Path path = new Path("./output/parquet/data_avro.parquet");
        AvroParquetWriter<GenericRecord> writer = new AvroParquetWriter<>(path, schema);
        writer.write(datum);
        writer.close();

        AvroParquetReader<GenericRecord> reader = new AvroParquetReader<>(path);
        GenericRecord result;
        while ((result = reader.read()) != null){
            System.out.println(result.get("left"));
            System.out.println(result.get("right"));
        }
    }

    @Test
    public void testParquetFileUsingAvroProjection() throws IOException {
        Path path = new Path("./output/parquet/data_avro.parquet");
        Schema.Parser parser = new Schema.Parser();
        Schema projectionSchema = parser.parse(getClass().getResourceAsStream("/avro/DeleteStringPair.avsc"));
        Configuration conf = new Configuration();
        AvroReadSupport.setRequestedProjection(conf, projectionSchema);
        AvroParquetReader<GenericRecord> reader = new AvroParquetReader<GenericRecord>(conf, path);
        GenericRecord result;
        while ((result = reader.read()) != null){
            System.out.println(result.get("left"));
            System.out.println(result.get("right"));
        }
    }

    @Test
    public void testParquetFileUsingAvroProjectionAndReadSchema() throws IOException {
        Path path = new Path("./output/parquet/data_avro.parquet");
        Schema.Parser parser = new Schema.Parser();
        Schema projectionSchema = parser.parse(getClass().getResourceAsStream("/avro/DeleteStringPair.avsc"));
        Schema readSchema = parser.parse(getClass().getResourceAsStream("/avro/NewStringPair.avsc"));
        Configuration conf = new Configuration();
        AvroReadSupport.setRequestedProjection(conf, projectionSchema);
        AvroReadSupport.setAvroReadSchema(conf, readSchema);
        AvroParquetReader<GenericRecord> reader = new AvroParquetReader<GenericRecord>(conf, path);
        GenericRecord result;
        while ((result = reader.read()) != null){
            System.out.println(result.get("left"));
            System.out.println(result.get("right"));
            System.out.println(result.get("description"));
        }
    }

}

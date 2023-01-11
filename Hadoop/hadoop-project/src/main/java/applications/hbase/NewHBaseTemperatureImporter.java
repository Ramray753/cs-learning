package applications.hbase;

import mapreduce.application.NcdcRecordParser;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.conf.Configured;
import org.apache.hadoop.hbase.HBaseConfiguration;
import org.apache.hadoop.hbase.TableName;
import org.apache.hadoop.hbase.client.Connection;
import org.apache.hadoop.hbase.client.ConnectionFactory;
import org.apache.hadoop.hbase.client.Put;
import org.apache.hadoop.hbase.client.Table;
import org.apache.hadoop.hbase.util.Bytes;
import org.apache.hadoop.io.IOUtils;
import org.apache.hadoop.util.Tool;
import org.apache.hadoop.util.ToolRunner;

import java.io.*;

public class NewHBaseTemperatureImporter extends Configured implements Tool {

    public int run(String[] args) throws IOException {
        if (args.length != 1) {
            System.err.println("Usage: HBaseStationImporter <input>");
            return -1;
        }

        Configuration config = HBaseConfiguration.create();
        try (Connection connection = ConnectionFactory.createConnection(config)) {
            // Create table
            TableName tableName = TableName.valueOf("observations");
            try (Table table = connection.getTable(tableName)) {
                BufferedReader in = null;
                File folder = new File(args[0]);
                File[] listOfFiles = folder.listFiles();
                assert listOfFiles != null;
                for (File file : listOfFiles) {
                    if (file.isFile()) {
                        try {
                            in = new BufferedReader(new InputStreamReader(new FileInputStream(file)));
                            NcdcRecordParser parser = new NcdcRecordParser();
                            String line;
                            while ((line = in.readLine()) != null) {
                                parser.parse(line);
                                if (parser.isValidTemperature()) {
                                    byte[] rowKey = RowKeyConverter.makeObservationRowKey(parser.getStationId(),
                                            parser.getObservationDate().getTime());
                                    Put p = new Put(rowKey);
                                    p.addColumn(NewHBaseTemperatureQuery.DATA_COLUMNFAMILY,
                                            NewHBaseTemperatureQuery.AIRTEMP_QUALIFIER,
                                            Bytes.toBytes(parser.getAirTemperature()));
                                    table.put(p);
                                }
                            }
                        } finally {
                            IOUtils.closeStream(in);
                        }
                    }
                }
            }
        }
        return 0;
    }

    public static void main(String[] args) throws Exception {
        int exitCode = ToolRunner.run(HBaseConfiguration.create(), new NewHBaseTemperatureImporter(), args);
        System.exit(exitCode);
    }
}
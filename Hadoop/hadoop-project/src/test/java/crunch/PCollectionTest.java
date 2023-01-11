package crunch;

import com.google.common.collect.Iterables;
import org.apache.crunch.*;
import org.apache.crunch.fn.Aggregators;
import org.apache.crunch.impl.mem.MemPipeline;
import org.apache.crunch.types.writable.Writables;
import org.junit.Test;

import java.io.Serializable;
import java.util.Iterator;

import static crunch.Utils.dump;

public class PCollectionTest implements Serializable {
    @Test
    public void test1() {
        PCollection<Integer> a = MemPipeline.collectionOf(1, 3);
        PCollection<Integer> b = MemPipeline.collectionOf(2);
        PCollection<Integer> c = a.union(b);
        System.out.println(Utils.dump(c));
    }

    @Test
    public void test2() {
        PCollection<String> a = MemPipeline.collectionOf("cherry", "apple", "banana");
        DoFn<String, Integer> doFn = new DoFn<String, Integer>() {
            @Override
            public void process(String input, Emitter<Integer> emitter) {
                emitter.emit(input.length());
            }
        };
        PCollection<Integer> b = a.parallelDo(doFn, Writables.ints());
        System.out.println(Utils.dump(b));
    }

    @Test
    public void test3() {
        PCollection<String> a = MemPipeline.collectionOf("cherry", "apple", "banana");
        MapFn<String, Integer> mapFn = new MapFn<String, Integer>() {
            @Override
            public Integer map(String input) {
                return input.length();
            }
        };
        PCollection<Integer> b = a.parallelDo(mapFn, Writables.ints());
        System.out.println(Utils.dump(b));
    }

    @Test
    public void test4() {
        PCollection<String> a = MemPipeline.collectionOf("cherry", "apple", "banana");
        FilterFn<String> filterFn = new FilterFn<String>() {
            @Override
            public boolean accept(String input) {
                return input.length() % 2 == 0; // even }
            }
        };
        PCollection<String> b = a.filter(filterFn);
        System.out.println(Utils.dump(b));
    }

    @Test
    public void test5() {
        PCollection<String> a = MemPipeline.collectionOf("cherry", "apple", "banana");
        DoFn<String, Pair<Integer, String>> doFn = new DoFn<String, Pair<Integer, String>>() {
            @Override
            public void process(String input, Emitter<Pair<Integer, String>> emitter) {
                emitter.emit(Pair.of(input.length(), input));
            }
        };
        PTable<Integer, String> b = a.parallelDo(doFn, Writables.tableOf(Writables.ints(), Writables.strings()));
        System.out.println(Utils.dump(b));
    }

    @Test
    public void test6() {
        PCollection<String> a = MemPipeline.typedCollectionOf(Writables.strings(), "cherry", "apple", "banana");
        MapFn<String, Integer> mapFn = new MapFn<String, Integer>() {
            @Override
            public Integer map(String input) {
                return input.length();
            }
        };
        PTable<Integer, String> b = a.by(mapFn, Writables.ints());
        System.out.println(Utils.dump(b));
    }

    @Test
    public void test7() {
        PCollection<String> a = MemPipeline.collectionOf("cherry", "apple", "banana");
        DoFn<String, Pair<Integer, String>> doFn = new DoFn<String, Pair<Integer, String>>() {
            @Override
            public void process(String input, Emitter<Pair<Integer, String>> emitter) {
                emitter.emit(Pair.of(input.length(), input));
            }
        };
        PTable<Integer, String> b = a.parallelDo(doFn, Writables.tableOf(Writables.ints(), Writables.strings()));
        PGroupedTable<Integer, String> c = b.groupByKey();
        System.out.println(Utils.dump(c));
    }

    @Test
    public void test8() {
        PCollection<String> a = MemPipeline.collectionOf("cherry", "apple", "banana");
        DoFn<String, Pair<Integer, String>> doFn = new DoFn<String, Pair<Integer, String>>() {
            @Override
            public void process(String input, Emitter<Pair<Integer, String>> emitter) {
                emitter.emit(Pair.of(input.length(), input));
            }
        };
        PTable<Integer, String> b = a.parallelDo(doFn, Writables.tableOf(Writables.ints(), Writables.strings()));
        PGroupedTable<Integer, String> c = b.groupByKey();
        CombineFn<Integer, String> combineFn = new CombineFn<Integer, String>() {
            @Override
            public void process(Pair<Integer, Iterable<String>> input, Emitter<Pair<Integer, String>> emitter) {
                StringBuilder sb = new StringBuilder();
                for (Iterator i = input.second().iterator(); i.hasNext(); ) {
                    sb.append(i.next());
                    if (i.hasNext()) {
                        sb.append(";");
                    }
                }
                emitter.emit(Pair.of(input.first(), sb.toString()));
            }
        };
        PTable<Integer, String> d = c.combineValues(combineFn);
        System.out.println(Utils.dump(d));
    }

    @Test
    public void test9() {
        PCollection<String> a = MemPipeline.collectionOf("cherry", "apple", "banana");
        DoFn<String, Pair<Integer, String>> doFn = new DoFn<String, Pair<Integer, String>>() {
            @Override
            public void process(String input, Emitter<Pair<Integer, String>> emitter) {
                emitter.emit(Pair.of(input.length(), input));
            }
        };
        PTable<Integer, String> b = a.parallelDo(doFn, Writables.tableOf(Writables.ints(), Writables.strings()));
        PGroupedTable<Integer, String> c = b.groupByKey();
        PTable<Integer, String> e = c.combineValues(Aggregators.STRING_CONCAT(";", false));
        System.out.println(Utils.dump(e));
    }

    @Test
    public void test10() {
        PCollection<String> a = MemPipeline.collectionOf("cherry", "apple", "banana");
        DoFn<String, Pair<Integer, String>> doFn = new DoFn<String, Pair<Integer, String>>() {
            @Override
            public void process(String input, Emitter<Pair<Integer, String>> emitter) {
                emitter.emit(Pair.of(input.length(), input));
            }
        };
        PTable<Integer, String> b = a.parallelDo(doFn, Writables.tableOf(Writables.ints(), Writables.strings()));
        PGroupedTable<Integer, String> c = b.groupByKey();
        MapFn<Iterable<String>, Integer> mapFn = new MapFn<Iterable<String>, Integer>() {
            @Override
            public Integer map(Iterable<String> input) {
                return Iterables.size(input);
            }
        };
        PTable<Integer, Integer> f = c.mapValues(mapFn, Writables.ints());
        System.out.println(Utils.dump(f));
    }
}

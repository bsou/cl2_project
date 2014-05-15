import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.Writer;
import java.util.*;

public class BuildVocab {

	/**
	 * @param args
	 */
	
	private String filename;
	private Map<String, Integer> vocablist;
	
	public BuildVocab(String fname) {
		filename = fname;
		vocablist = new HashMap<String, Integer>();
	}
	
	public void constructVocab() {
		try {
			BufferedReader br = new BufferedReader(new FileReader(filename));
			String line, word;
			int linecount = 0;
			while ((line = br.readLine()) != null) {
				++linecount;
				word = line.trim().toLowerCase();
				if(!vocablist.containsKey(word)) {
					vocablist.put(word, 1);
				}
			}
			
			br.close();
		} catch(Exception e) {
			System.err.println(" Read Error: " + e.getMessage());
		}
	}
	
	public void printFile(String writefname) {
		Writer writer = null;

		try {
			System.out.println("Writing file: " + writefname);
			writer = new BufferedWriter(new OutputStreamWriter(
					new FileOutputStream(writefname), "utf-8"));

			Iterator<Map.Entry<String, Integer>> i = vocablist.entrySet().iterator(); 
			while(i.hasNext()){
			    String key = i.next().getKey();
			    writer.write(key + "\n");
			}

		} catch (IOException ex) {
			System.err.println("Write Error: " + ex.getMessage());
		} finally {
			try {writer.close();} catch (Exception ex) {}
		}
	}
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		final String fname = "data/words";
		final String wfname = "data/vocab.txt";
		BuildVocab bv = new BuildVocab(fname);
		bv.constructVocab();
		bv.printFile(wfname);
	}

}

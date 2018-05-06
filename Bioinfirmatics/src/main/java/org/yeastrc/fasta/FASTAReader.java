/*
 *   Copyright 2015 Michael Riffle
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

package org.yeastrc.fasta;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.HashSet;
import java.util.Set;

/**
 * This class iterates over a FASTA file by calling readNext to return
 * FASTAEntry objects.
 */

public class FASTAReader {

	/**
	 * Get an instance of this class
	 * @param filename The filename of the FASTA file to read
	 * @return
	 * @throws Exception If there is a problem
	 */
	public static FASTAReader getInstance( String filename ) throws Exception {

		if (filename == null)
			throw new IllegalArgumentException( "filename may not be null" );

		return getInstance( new File( filename ) );
	}

	/**
	 * Get an instance of this class
	 * @param filename The filename of the FASTA file to read
	 * @return
	 * @throws Exception If there is a problem
	 */
	public static FASTAReader getInstance( File file ) throws Exception {


		if ( file == null ) {
			
			throw new IllegalArgumentException( "file may not be null" );
		}
		
		if ( ! file.exists() ) {
			
			throw new IllegalArgumentException( "File does not exist: " + file.getAbsolutePath() );
		}
		
		FileInputStream fileInputStream = new FileInputStream( file );
		
		return getInstance( fileInputStream );
	}


	/**
	 * Get an instance of this class
	 * @param is An InputStream for the FASTA data
	 * @return
	 * @throws Exception If there is a problem
	 */
	public static FASTAReader getInstance( InputStream inputStream ) throws Exception {

		if (inputStream == null)
			throw new IllegalArgumentException( "inputStream may not be null" );


		FASTAReader reader = new FASTAReader();

		InputStreamReader isr = new InputStreamReader( inputStream, FASTAReaderConstants.inputCharSet );
		reader.br = new BufferedReader( isr );

		return reader;
	}

	/**
	 * Close the connection with the FASTA file
	 * @throws Exception
	 */
	public void close() throws Exception {
		if( this.br != null )
			this.br.close();

		this.br = null;
	}


	/**
	 * Get the next entry in the FASTA file. Returns null if end of file has been reached.
	 *
	 * @return
	 * @throws FASTADataErrorException for data errors
	 * @throws Exception
	 */
	public FASTAEntry readNext() throws FASTADataErrorException, Exception {

		/*
		 * It is assumed the last read correctly returned a Set of headers and a sequence
		 * So, it is therefor assumed the BufferedReader's next line read will be a header line
		 * followed by sequence lines (unless last read returned false (end of file) )
		 */

		String line = null;
		if( this.lineNumber == 0 )
			this.lastLineRead = this.br.readLine();

		line = this.lastLineRead;

		if (line == null) return null;			// we've reached the end of the file
		this.lineNumber++;

		int headerLineNumber = this.lineNumber;
		
		if (!line.startsWith( ">" ) )
			throw new FASTADataErrorException( "Line Number: " + this.lineNumber + " - Expected header line, but line did not start with \">\"." );

		String headerLine = line;

		// the headers for this entry
		Set<FASTAHeader> headers = new HashSet<FASTAHeader>();
		StringBuilder sequence = new StringBuilder();

		line = line.substring(1, line.length());	// strip off the leading ">" on the header line

		/*
		 * In FASTA files, multiple headers can be associated with the same sequence, and will
		 * be present on the same line.  The separate headers are separated by the CONTROL-A
		 * character, so we split on that here, and save each to the headers Set
		 */
		String[] lineHeaders = line.split("\\cA");
		for (int i = 0; i < lineHeaders.length; i++) headers.add( new FASTAHeader( lineHeaders[i] ) );

		// The next line must be a sequence line
		line = this.br.readLine();
		this.lastLineRead = line;

		while (line.startsWith( ";" )) {
			this.lineNumber++;
			line = this.br.readLine();
			this.lastLineRead = line;
		}
		if (line == null || line.startsWith( ">" ))
			throw new FASTADataErrorException( "Did not get a sequence line after a header line (Line Number: " + this.lineNumber );


		// loop through the file, reading sequence lines until we hit the next header line (or the end of the file)
		while (line != null) {

			//If we've reached a new header line (marked with a leading ">"), then we're done.
			if (line.startsWith( ">" )) {
				break;
			}

			this.lineNumber++;

			// build the sequence, if it's not a comment line
			if (!line.startsWith( ";" )) {

				// upper-case the sequence line
				line = line.toUpperCase();

				sequence.append( line );
			}

			line  = this.br.readLine();
			this.lastLineRead = line;
		}
		
		String sequenceString = sequence.toString();
		
		sequenceString = sequenceString.trim();

		// If we've made it here, we've read another sequence entry in the FASTA data
		return new FASTAEntry( headers, sequenceString, headerLine, headerLineNumber );
	}

	private BufferedReader br;
	private int lineNumber;
	private String lastLineRead;
}

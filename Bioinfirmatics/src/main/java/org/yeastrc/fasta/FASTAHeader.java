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

import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Represents a header associated with a sequence in the FASTA file
*/
public final class FASTAHeader {

	public int hashCode() {
		return this.line.hashCode();
	}

	public boolean equals( Object o ) {
		if( !( o instanceof FASTAHeader ) ) return false;

		return ((FASTAHeader)o).getLine().equals( this.getLine() );
	}


	/**
	 * Constructor that parses a single header into it's parts
	 * @param line
	 * @throws Exception
	 */
	public FASTAHeader( String line ) throws FASTADataErrorException {

		Pattern withoutDescription = Pattern.compile("^(\\S+)\\s*$");
		Pattern withDescription    = Pattern.compile("^(\\S+)\\s+(\\S+.*)$");

		Matcher m = withoutDescription.matcher( line );
		if( m.matches() ) {
			this.line = line;
			this.name = m.group( 1 );
			this.description = null;
		} else {
			m = withDescription.matcher( line );
			if( m.matches() ) {
				this.name = m.group( 1 );
				this.description = m.group( 2 );
				this.line = line;
			} else {
				throw new FASTADataErrorException( "Could not parse FASTA header line: \"" + line + "\"" );
			}
		}
	}
	

	/**
	 * Constructor that creates a new object from the provided parts
	 * 
	 * @param name
	 * @param description
	 * @param line
	 * @throws Exception
	 */
	public FASTAHeader( String name, String description, String line ) {
		
		this.name = name;
		this.description = description;
		this.line = line;
	}

	public String getName() {
		return name;
	}
	public String getDescription() {
		return description;
	}
	public String getLine() {
		return line;
	}

	private final String name;
	private final String description;
	private final String line;
}

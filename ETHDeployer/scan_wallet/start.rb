require 'json'
require 'open-uri'
require 'find'
class ScanWallet

	def scan_gene(min_num)
		#@address_json.each do |address,value|
			scan_add_address("0x(your eth address)") #if value['value'] >= min_num
		#end
		#puts file_list.last
	end

	private

		def scan_add_address(address)
			@new_address_arr = ""
			address_list =  File.read("gene_list/address_list_2")
			st = get_gsc_tx(address)
			address = []
			body = JSON.parse st
			body['result'].each do |b1|
				if b1['value'].to_s.to_i < "10000000000000000000".to_i && !(address_list.include? b1['to'])
					@new_address_arr = @new_address_arr + "#{b1['to']},1\n"
					puts b1['to']
				end
			end
			@new_address_arr = address_list + @new_address_arr
			file = File.new("gene_list/address_list_2", "w")
			File.open(file, 'w+'){|f| f << @new_address_arr.to_s}
		end
		
		def get_gsc_tx(address)
			etherscan_api = "（etherscan_api_key）"
			contract = "(your contract address)"
			url = "http://api.etherscan.io/api?module=account&action=tokentx&contractaddress=#{contract}&address=#{address}&page=7&offset=9999&sort=asc&apikey=#{etherscan_api}"
			ruby_get(url)
		end

		def ruby_get(uri)
	    html_response = nil  
	    open(uri) do |http|  
	      html_response = http.read  
	    end 
	    puts html_response
	    html_response
	  end
end

sw = ScanWallet.new 
sw.scan_gene(0)




